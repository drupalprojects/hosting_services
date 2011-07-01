<?php

/**
 * Implementation of hook_user_default_roles().
 */
function hosting_services_restful_user_default_roles() {
  $roles = array();

  // Exported role: aegir remote manager (RESTful)
  $roles['aegir remote manager (RESTful)'] = array(
    'name' => 'aegir remote manager (RESTful)',
  );

  return $roles;
}
