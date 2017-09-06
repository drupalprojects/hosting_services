<?php

/**
 * List all sites.
 */
function hosting_services_list_sites() {
  $sites = array();
  $site_result = db_query("SELECT hs.*, node.title AS title FROM {hosting_site} hs INNER JOIN {node} ON hs.nid = node.nid WHERE hs.status <> :status_deleted", array(':status_deleted' => HOSTING_SITE_DELETED))->fetchAll();

  foreach ($site_result as $site) {
    $sites[] = $site;
  }

  return $sites;
}

/**
 * Create sites.
 */
function hosting_services_create_site($url, $data) {
  $site = hosting_services_save_site($url, $data);
  if (!empty($site->nid)) {
    hosting_add_task($site->nid, 'verify');
    return $site;
  }
  return FALSE;
}

/**
 * Update sites.
 */
function hosting_services_save_site($url, $data) {
  global $user;
  // Try to retrieve an existing site.
  $site = hosting_get_site_by_url(check_plain($url));
  if ($site) {
    $site = (object) array_merge((array) $site, $data);
  }
  else {
    if (!is_numeric($data['platform']) || !is_numeric($data['profile'])) {
      return FALSE;
    }
    $site = new stdClass();
    $site->type = 'site';
    $site->title = $site->hosting_name = check_plain($url);
    $site->db_server = (is_numeric($data['db_server'])) ? $data['db_server'] : variable_get('hosting_default_db_server', 0);
    $site->client = ((isset($data['client']) && is_numeric($data['client']))) ? $data['client'] : variable_get('hosting_default_client', 1);
    $site = (object) array_merge((array) $site, $data);
    $site->uid = $user->uid;
    $site->site_language = isset($data['language']) ? $data['language'] : 'en';
    // Provide IP addresses if none have been provided.
    if (empty($site->ip_addresses)) {
      $site->ip_addresses = _hosting_services_default_ip();
    }
  }

  drupal_alter('hosting_services_site_presave', $site);

  $form_state = array();
  node_validate($site, array(), $form_state);

  $site->status = 1;
  node_save($site);

  return $site;
}

/**
 * Create tasks.
 */
function hosting_services_site_task($type, $nid) {
  global $user;
  $tasks = hosting_available_tasks('site');
  if (array_key_exists($type, $tasks)) {
    $task = hosting_add_task($nid, $type);
    return $task;
  }
  else {
    return FALSE;
  }
}

/**
 * Helper function to retrieve a default server's IP addresses.
 */
function _hosting_services_default_ip() {
  $http = node_load(variable_get('hosting_default_web_server', 0));
  return $http->ip_addresses;
}

/**
 * This sends a login link to a site. The cached link would no longer be valid.
 */
function hosting_services_site_login($nid) {
  $cache = cache_get("hosting:site:" . $nid . ":login_link");
  $cid = "hosting:site:" . $nid . ":login_link";
  $cache = cache_get($cid);
  if (is_object($cache) && (REQUEST_TIME < $cache->data['expire'])) {
    $theurl = $cache->data['link'];
    return $theurl;
  }
  else {
    return '';
  }
}

/**
 * This tells the server the login link has been used and is no longer valid.
 */
function hosting_services_site_expire_login($nid) {
  $cache = cache_get("hosting:site:" . $nid . ":login_link");
  $cid = "hosting:site:" . $nid . ":login_link";
  $cache = cache_get($cid);
  if (is_object($cache)) {
    cache_clear_all($cid, 'cache');
  }
  else {
    return '';
  }
}
