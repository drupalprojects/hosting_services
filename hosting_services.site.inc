<?php 
/**
 * List all sites
 */
function hosting_services_list_sites() {
  $sites = array();
  $site_result = db_query('SELECT nid, title FROM {node} WHERE type = "site"');
  while ($row = db_fetch_array($site_result)) {
    $sites[] = $row;
  }
  return $sites;
}

/**
 * Create and update sites
 */
function hosting_services_save_site($url, $data) {
  global $user;
  // Try to retrieve an existing site
  $site = hosting_get_site_by_url(check_plain($url));
  if ($site) {
    $site = (object) array_merge((array) $site, $data);
  }
  else {
    if (!is_numeric($data['platform']) || !is_numeric($data['profile']) || !is_numeric($data['client'])) {
      return FALSE;
    }
    $site->type = 'site';
    $site->title = $site->hosting_name = check_plain($url);
    $site->db_server = variable_get('hosting_default_dbserver', $data['db_server']);
    $site->platform = $data['platform'];
    $site->profile = $data['profile'];
    $site->client = $data['client'];
    $site->uid = $user->uid;
  }

  node_validate($site);

  $site->status = 1;
  return node_save($site); 
}
