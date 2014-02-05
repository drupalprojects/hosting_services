<?php

/**
 * Returns a single Platform
 */
function hosting_services_profile_get($profile_id) {
  $output = array(
    'profile' => node_load($profile_id),
    'platforms' => hosting_get_profile_platforms($profile_id),
  );

  return $output;
}
