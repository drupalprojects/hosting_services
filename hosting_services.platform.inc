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
  $result = db_query("SELECT nid FROM {hosting_site} WHERE platform = :platform", array(':platform' => $nid));
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
