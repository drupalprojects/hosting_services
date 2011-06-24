<?php 

/**
 * Returns a single Platform
 */
function hosting_services_profile_get($profile_id) {
  $profile = node_load(array('nid' => $profile_id, 'type' => 'package'));
  return $profile;
}
