<?php 

/**
 * Perform site.save
 */
function hosting_services_save_site($xtras, $url, $server, $platform, $profile, $client, $site_options, $uid){
  watchdog('Aegir Remoting', 'xtras:'.$newsite
    .' URL:'.$url
    .' Server:'.$server
    .' Platform:'.$platform
    .' Profile:'.$profile
    .' Client:'.$client
    .' Options:'.$site_options
    .' Client:'.$client
  );
  $site->type = 'site';
  $site->title = $url;
  $site->db_server = variable_get( 'hostmaster_prefered_dbserver', $server );
  $site->platform = $platform;
  $site->profile = $profile;
  $site->hostmaster_site_options = $site_options;
  $site->uid = $uid;
  $clientCheck = db_fetch_object(db_query('SELECT * FROM {hosting_client} WHERE email = "%s"', $client));
  if(!isset($clientCheck->nid)){
    watchdog('Aegir Remoting', 'Could not find a client with an email of '.$client);
    $clientNode->email = $client;
    $clientNode->email_confirm = $client;
    $clientNode->uid = $uid;
    $clientNode->client_name = $client;
    $clientNode->type = 'client';
    $clientSaved = node_save($clientNode);
    $clientCheck = db_fetch_object(db_query('SELECT * FROM {hosting_client} WHERE mail = "%s"', $client));
    $site->client = $client;
  } 
  else {
    // We found the client so we are good to go
    $site->client = $client;
  }
  node_save($site); 
  return $site;
}

/**
 * Return aegir's site node
 */
function hosting_services_get_site($sessid, $url ){
  $site = node_load(array('title' => $url));
  return $site;
}
