<?php

/**
 * Perform client.sites
 *
 * @param $id
 *   Nid or unique client name suitable for use with hosting_get_client
 */
function hosting_services_client_sites($id = ''){
  $client = hosting_get_client($id);
  if($client && !empty($client)){
    $siteList = db_query("SELECT * FROM {hosting_site} WHERE client = :nid", array(':nid' => $client->nid))->fetchAll();
    $sites = array();
    foreach($siteList as $site){
      $sites[] = node_load($site->nid);
    }
    return $sites;
  }
  else {
    // Client does not exist in the system
    return false;
  }
}

/**
 * Check to make sure the user has the right to do things to this client
 *
 * @param $name
 *   The client name or nid
 */
function hosting_services_client_access($name) {
  global $user;
  $client = hosting_get_client($name);
  if ($client && (user_access('administer clients') || array_key_exists($client->nid, hosting_get_client_from_user($user->uid)))) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Save a client
 *
 * @param $name
 *   The name or nid of the client
 */
function hosting_services_client_save($name, $data = NULL){
  global $user;

  if (!is_array($data)) {
    $client = new stdClass();

    $client->type = 'client';
    $client->title = $name;
    $client->uname = hosting_client_sanitize($name);
    $client->uid = ($user->uid) ? $user->uid : NULL;
    $client->name = ($user->name) ? $user->name : NULL;

    // Make sure the client title is unique
    $check = db_query("SELECT nid FROM {hosting_client} WHERE uname LIKE ':name'", array(':name' => addcslashes($client->uname, '\%_')))->fetchField();
    if ($check) {
      $client->title = hosting_client_validate_suggest($client);
    }
  }
  else {
    $client = hosting_get_client($name);
    $client = (object) array_merge((array) $client, $data);
  }

  // Validate the node
  node_validate($client);

  // Save the node
  $client->status = 1;
  node_save($client);

  if(variable_get('hosting_client_register_user', FALSE)){
    $created_account = user_load($client->uid, TRUE);
    $client->account = $created_account;
  }

  return !empty($client->nid) ? $client : FALSE;
}

/**
 * Delete a client
 *
 * @param $name
 *   Client name or nid
 */
function hosting_services_client_delete($name) {
  $client = hosting_get_client($name)
    ? hosting_get_client($name)
    : hosting_get_client_by_uname($name);

  if ($client) {
    node_delete($client->nid);
    return TRUE;
  }
  return FALSE;
}

/**
 * Suspend client sites
 */
function hosting_services_suspend_client_sites($name){
  $client = hosting_get_client($name);
  if (!$client) {
    return FALSE;
  }
  $siteList = db_query("SELECT * FROM {hosting_site} WHERE client = :nid", array(':nid' => $client->nid))->fetchAll();
  $tasks = array();
  foreach($siteList as $site){
    $tasks[] = hosting_add_task($site->nid, 'disable');
  }
  return $tasks;
}

/**
 * Unsuspend a clients sites
 */
function hosting_services_unsuspend_client_sites($name){
  $client = hosting_get_client($name);
  if (!$client) {
    return FALSE;
  }
  $siteList = db_query("SELECT * FROM {hosting_site} WHERE client = :nid", array(':nid' => $client->nid))->fetchAll();
  $tasks = array();
  foreach($siteList as $site){
    $tasks[] = hosting_add_task($site->nid, 'enable');
  }
  return $tasks;
}
