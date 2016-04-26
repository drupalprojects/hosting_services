<?php

/**
 * Implementation of hook_default_services_endpoint().
 */
function hosting_services_restful_default_services_endpoint() {
  $export = array();
  $endpoint = new stdClass;
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'restful';
  $endpoint->title = 'RESTful';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'restful';
  $endpoint->authentication = array(
    'services_sessauth' => 'services_sessauth',
  );
  $endpoint->resources = array(
    'hosting_profile' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'hosting_platform' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'hosting_task' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'hosting_site' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'hosting_client' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
      ),
      'actions' => array(
        'sites' => array(
          'enabled' => 1,
        ),
        'disable_sites' => array(
          'enabled' => 1,
        ),
        'enable_sites' => array(
          'enabled' => 1,
        ),
      ),
    ),
  );
  $endpoint->debug = 0;

  $export['restful'] = $endpoint;
  return $export;
}
