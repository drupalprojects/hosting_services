<?php 

// Access callbacks follow

function get_client_access($email){
  global $user;
  return true; //('get client', $user );
}

function client_site_access($nid){
  global $user;
  return user_access('load client data');
}

function save_client_access($email, $name){
  global $user;
  return user_access('save client', $user);
}


/**
 * Perform client.get
 */
function hosting_services_client($email){
  watchdog('Aegir Services', 'Looking up client with email:'.$email);
  if(db_result(db_query("SELECT COUNT(*) FROM {hosting_client} WHERE email = '%s'", $email)) !== 0){
    watchdog('Aegir Services', 'Located user matching the provided email '.$email);
    // Load and return the client Node
    $client = db_fetch_object(db_query("SELECT nid FROM {hosting_client} WHERE email = '%s'", $email ) );  
    $node = services_node_load(array('nid' => $client->nid ) );
    return $node;
  } 
  else {
    return services_error(t('Could not find the client.'), 404);
  }
  
}

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
 * Create a new client node
 */
function hosting_services_client_save($name = ''){
  global $user;

  $client = new stdClass();

  $client->type = 'client';
  $client->title = $name;
  $client->uid = ($user->uid) ? $user->uid : NULL;
  $client->name = ($user->name) ? $user->name : NULL;

  // Make sure the client title is unique
  if ($suggestion = hosting_client_validate_suggest($client)) {
    $client->title = $suggestion;
  }

  // Validate the node
  node_validate($client);

  // Save the node
  $client->status = 1;
  $new_client = node_save($client);

  return $new_client;
}




/**
 * Suspend client sites
 */

function hosting_services_suspend_client_sites($email, $message =''){
  $client = db_fetch_object(db_query("SELECT * FROM {hosting_client} WHERE email = '%s'", $email));
  $siteList = db_query("SELECT * FROM {hosting_site} WHERE client = %d", $client->nid);
  $task->type = 'task';
  $task->task_type = 'disable';
  
  while($site = db_fetch_object($siteList)){
    $task->rid = $site->nid;  
    node_save($task);
    $siteNode = node_load($site->nid);
    $siteNode->disabled_by_fimafeng = 1;
    $siteNode->disabled_explanation = $message;
    node_save($siteNode);
  }
  return TRUE;
}



/**
 * Unsuspend a clients sites
 */
function hosting_services_unsuspend_client_sites($email){
  $client = db_fetch_object(db_query("SELECT * FROM {hosting_client} WHERE email = '%s'", $email));
  $siteList = db_query("SELECT * FROM {hosting_site} WHERE client = %d", $client->nid);
  $task->type = 'task';
  $task->task_type = 'enable';
  
  while($site = db_fetch_object($siteList)){
    $task->rid = $site->nid;  
    node_save($task);
    $siteNode = node_load($site->nid);
    $siteNode->disabled_by_fimafeng = 0;
    $siteNode->disabled_explanation = '';
    node_save($siteNode);  
  }
}
