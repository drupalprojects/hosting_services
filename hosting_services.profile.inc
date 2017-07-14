<?php

/**
 * @file
 * Installation profile operations.
 */

/**
 * List all available (i.e. non-blocked) installation profiles.
 */
function hosting_services_profile_list($machine_names = FALSE) {
  $profile_titles = hosting_get_profiles();

  if ($machine_names) {
    $profile_machine_names = hosting_get_profiles(NULL, 'short_name');
    return array_combine($profile_machine_names, $profile_titles);
  }

  return $profile_titles;
}

/**
 * Returns a single Platform.
 */
function hosting_services_profile_get($profile_id) {
  $output = array(
    'profile' => node_load($profile_id),
    'platforms' => hosting_get_profile_platforms($profile_id),
  );

  return $output;
}
