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
    $site_list_qry = db_query("SELECT * FROM {hosting_site} WHERE client = %d", $client->nid);
    $sites = array();
    while($site = db_fetch_object($site_list_qry) ){
      $sites[] = node_load(array('nid' => $site->nid ));       
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
    $client->uid = ($user->uid) ? $user->uid : NULL;
    $client->name = ($user->name) ? $user->name : NULL;

    // Make sure the client title is unique
    if ($suggestion = hosting_client_validate_suggest($client)) {
      $client->title = $suggestion;
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
  $new_client = node_save($client);

  return $new_client;
}

/**
 * Delete a client
 *
 * @param $name
 *   Client name or nid
 */
function hosting_services_client_delete($name) {
  if (!is_numeric($name)) {
    $client = hosting_get_client($name);
    node_delete($client->nid);
  }
  else {
    node_delete($name);
  }
}

/**
 * Suspend client sites
 */

function hosting_services_suspend_client_sites($name){
  $client = hosting_get_client($name);
  if (!$client) {
    return FALSE;
  }
  $siteList = db_query("SELECT * FROM {hosting_site} WHERE client = %d", $client->nid);
  $task->type = 'task';
  $task->task_type = 'disable';
  
  while($site = db_fetch_object($siteList)){
    $task->rid = $site->nid;  
    node_save($task);
  }
  return TRUE;
}

/**
 * Unsuspend a clients sites
 */
function hosting_services_unsuspend_client_sites($name){
  $client = hosting_get_client($name);
  if (!$client) {
    return FALSE;
  }
  $siteList = db_query("SELECT * FROM {hosting_site} WHERE client = %d", $client->nid);
  $task->type = 'task';
  $task->task_type = 'enable';
  
  while($site = db_fetch_object($siteList)){
    $task->rid = $site->nid;  
    node_save($task);
  }
}
