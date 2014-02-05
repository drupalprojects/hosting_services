<?php
/**
 * Create or update a task
 */
function hosting_services_save_task($nid = NULL, $data = NULL, $options = NULL, $rid = NULL, $type = NULL) {

  $check_nid = db_query('SELECT type FROM {node} WHERE nid = :nid LIMIT 1', array(':nid' => $nid))->fetchField();

  if ($check_nid && $check_nid == 'task') {
    $task = node_load($nid);
    if ($task) {
      $task = (object) array_merge((array) $task, $data);
    }
    node_save($task);
  }
  else {
    $r_node = node_load($rid);
    $args = array();
    if ($r_node) {
      switch ($type) {
        case 'clone':
          $args = array(
            'target_platform' => $r_node->platform,
            'new_db_server' => $r_node->db_server,
            'new_uri' => $options['clone_url'],
          );
          break;
        default:
          break;
      }
      $task = hosting_add_task($r_node->nid, $type, $args);
    }
  }
  return $task;
}

function hosting_services_site_task($type,  $nid) {
  global $user;
  $tasks = hosting_available_tasks('site');
  if(array_key_exists($type, $tasks)){
    $task = hosting_add_task($nid, $type);
    return $task;
  }
  else{
    return FALSE;
  }
}

/**
 * Get task status
 */
function hosting_services_get_task($nid){
  return node_load($nid);
}
