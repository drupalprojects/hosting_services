<?php 

/**
 * Returns a single Platform
 */
function hosting_services_profile_get($profileID){
  return node_load(array('nid' => $profileID, 'type' => 'package'));
}
