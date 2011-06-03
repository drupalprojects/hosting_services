<?php 
// Access Callbacks
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


/*
 * Perform client.get
 */
function hostmaster_services_client($email){
	watchdog('Aegir Services', 'Looking up client with email:'.$email);
	if(db_result(db_query("SELECT COUNT(*) FROM {hosting_client} WHERE email = '%s'", $email)) !== 0){
		watchdog('Aegir Services', 'Located user matching the provided email '.$email);
		// Load and return the client Node
		$client = db_fetch_object(db_query("SELECT nid FROM {hosting_client} WHERE email = '%s'", $email ) );	
		$node = services_node_load(array('nid' => $client->nid ) );
		return $node;
	} else {
		return services_error(t('Could not find the client.'), 404);
	}
	
}

/*
 * Perform client.sites
 */
 function aegir_service_client_sites($email = ''){
 	if(db_result(db_query("SELECT COUNT(*) FROM {hosting_client} WHERE email = '%s'", $email)) > 0){
 		$client = db_fetch_object(db_query("SELECT * FROM {hosting_client} WHERE email ='%s'", $email));
 		$siteListQry = db_query("SELECT * FROM {hosting_site} WHERE client = %d", $client->nid);
 		$sites = array();
 		while($site = db_fetch_object($siteListQry) ){
 			$sites[] = node_load(array('nid' => $site->nid )); 			
 		}
 		return $sites;
 	} else {
 		// Client does not Exist in the system
 		return false;
 	}		
 }

/*
 * Create a new client node
 */
 function aegir_service_client_save($email = '', $name ='' ){
 	global $user;
 	$client->title = $email;
	$client->email_confirm =$email;
	$client->email = $email;
	$client->client_name = $email;
	$client->type = 'client';
	
 	$new = node_save( $client );
 	print_r($new);
 	return $new;
 }

/*
 * Suspend Client Sites
 */

function suspendClientSites($email, $message =''){
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



/*
 * unsuspends a clients sites
 */
function unsuspendClientSites($email){
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


?>