<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function hosting_services_restful_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer clients
  $permissions['administer clients'] = array(
    'name' => 'administer clients',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: create client
  $permissions['create client'] = array(
    'name' => 'create client',
    'roles' => array(
      '0' => 'aegir account manager',
      '1' => 'aegir remote manager (RESTful)',
      '2' => 'anonymous user',
    ),
  );

  // Exported permission: create task
  $permissions['create task'] = array(
    'name' => 'create task',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: get platform
  $permissions['get platform'] = array(
    'name' => 'get platform',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: get profile
  $permissions['get profile'] = array(
    'name' => 'get profile',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: get site
  $permissions['get site'] = array(
    'name' => 'get site',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: get task
  $permissions['get task'] = array(
    'name' => 'get task',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: list platforms
  $permissions['list platforms'] = array(
    'name' => 'list platforms',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: list profiles
  $permissions['list profiles'] = array(
    'name' => 'list profiles',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: save site
  $permissions['save site'] = array(
    'name' => 'save site',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: suspend client sites
  $permissions['suspend client sites'] = array(
    'name' => 'suspend client sites',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: unsuspend client sites
  $permissions['unsuspend client sites'] = array(
    'name' => 'unsuspend client sites',
    'roles' => array(
      '0' => 'aegir remote manager (RESTful)',
      '1' => 'anonymous user',
    ),
  );

  // Exported permission: view client
  $permissions['view client'] = array(
    'name' => 'view client',
    'roles' => array(
      '0' => 'aegir account manager',
      '1' => 'aegir client',
      '2' => 'aegir remote manager (RESTful)',
      '3' => 'anonymous user',
    ),
  );

  return $permissions;
}
