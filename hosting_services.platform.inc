<?php

/**
 * Enables Remote Accessible platform listing
 *
 * TODO Make use of the $rows and $offset parameters
 */
function hosting_services_platform_list($rows = 0, $offset = 0){
  $conditions = '';

  if (is_numeric($rows)) {
    $conditions .= ' LIMIT ' . check_plain($rows);
  }
  if ($conditions && is_numeric($offset)) {
    $conditions .= ' OFFSET ' . check_plain($offset);
  }
  $platform_query = db_query('SELECT n.nid, n.title FROM {node} n INNER JOIN {hosting_platform} hp ON hp.nid = n.nid WHERE n.type = :platform AND hp.status = :status' . $conditions, array(':platform' => 'platform', ':status' => HOSTING_PLATFORM_ENABLED));
  $result = $platform_query->fetchAll();

  $platforms = array();
  foreach ($result as $platform){
    $hsp_query = db_query('SELECT nid FROM {hosting_services_platforms} WHERE nid = :nid', array(':nid' => $platform->nid));

    if ($hsp_query->fetchAssoc()) {
      $platform->remote_site_creation_enabled = TRUE;
    } else {
      $platform->remote_site_creation_enabled = FALSE;
    }

    $platforms[] = $platform;
  }

  return $platforms;
}

/**
 * Create and update platforms.
 */
function hosting_services_save_platform($name, $data) {
  global $user;
  $name = check_plain($name);

  $platforms = array_flip(_hosting_get_platforms());
  if (isset($platforms[$name])) {
    $platform = node_load($platforms[$name]);
    $platform = (object) array_merge((array) $platform, $data);
  }
  else {
    if (!is_string($data['makefile']) && !is_string($data['git_repo'])) {
      return FALSE;
    }
    $platform->type = 'platform';
    $platform->title = $name;
    $platform = (object) array_merge((array) $platform, $data);
    $platform->uid = $user->uid;
    if (!isset($platform->web_server)) {
      $platform->web_server = HOSTING_DEFAULT_WEB_SERVER;
    }
    if (!isset($platform->publish_path)) {
      $platform->publish_path = variable_get('hosting_platform_base_path', '/var/aegir/platforms/') . $name;
    }
  }

  node_validate($platform);

  $platform->status = 1;
  node_save($platform);
  return !empty($platform->nid) ? $platform : FALSE;
}


/**
 * Returns a specific platform
 */
function hosting_services_platform_get($nid){
  $platform = node_load($nid);
  return $platform;
}

/**
 * Returns a specific platform
 */
function hosting_services_platform_get_sites($nid){
  $nodes = array();
  $result = db_query("SELECT nid FROM {hosting_site} WHERE platform = :platform AND status != :status_deleted", array(':platform' => $nid, ':status_deleted' => HOSTING_SITE_DELETED));
  while ($row = $result->fetchObject()) {
    $nodes[$row->nid] = node_load($row->nid);
  }

  return $nodes;
}

/**
 * Create tasks
 */
function hosting_services_platform_task($type,  $nid) {
  global $user;
  $tasks = hosting_available_tasks('platform');
  if(array_key_exists($type, $tasks)){
    $task = hosting_add_task($nid, $type);
    return $task;
  }
  else{
    return FALSE;
  }
}
