<?php 

/*
 * Implementation of Hook_nodeapi
 */
function hostmaster_services_nodeapi(&$node, $op, $a3 = NULL, $a4 = NULL){
  
    switch($node->type){
      
        case 'platform':
          // Lets define if we can create new sites on this platform from remote
          switch($op){
                  case 'insert':
                  if( $node->remote_site_creation_enabled ){
                      db_query('INSERT INTO {hostmaster_services_platforms} (vid, nid) VALUES (%d, %d)', $node->vid, $node->nid);
      			}
                  break;
                  case 'update':
                      if($node->revision && $node->remote_site_creation_enabled){
                          db_query('INSERT INTO {hostmaster_services_platforms} (vid, nid) VALUES (%d, %d)', $node->vid, $node->nid);
                      } else if( db_result( db_query( "SELECT COUNT(*) FROM {hostmaster_services_platforms} WHERE nid = %d AND vid = %d", $node->nid, $node->vid ) > 0 && $node->services_enabled == 0 ) ) {
                          db_query("DELETE FROM {hostmaster_services_platforms} WHERE nid = %d", $node->nid);
                      } else if($node->services_enabled == 0 ){
                          db_query("DELETE FROM {hostmaster_services_platforms} WHERE nid = %d", $node->nid);
                      }
                  break;
      			case 'delete':
      				db_query('DELETE FROM {hostmaster_services_platforms} WHERE nid =%d',$node->nid);
      			break;
      			case 'delete_revision':
      				db_query('DELETE FROM {hostmaster_services_platforms} WHERE vid = %d', $node->vid);
      			break;
      			case 'load':
      				if(db_result(db_query('SELECT COUNT(*) FROM {hostmaster_services_platforms} 
      				WHERE nid = %d', $node->nid)) > 0){
      					$node->remote_site_creation_enabled = 1;
      				} else {
      					$node->remote_site_creation_enabled = 0;
      				}
      			break;
      			case 'view':
      			
      			break;
      		}
        break;
    }
}

/*
 * Implementation of hook_form_alter
 */
function hostmaster_services_form_alter(&$form, &$form_state){
	if($form['#token'] == 'platform_node_form'){
		$form['remote_site_creation_enabled'] = array(
			'#type' => 'checkbox',
			'#title' => t('Enable this Platform for Remote Site Creation'),
			'#description' => t('Enables a platform to be used when creating sites via services.'),
			'#default_value' => $form['#node']->remote_site_creation_enabled,
		);
	} 	
}

?>