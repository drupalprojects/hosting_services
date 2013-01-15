<?php 
/**
 * Create or update a task
 */
function hosting_services_save_task($nid = NULL, $data = NULL, $options = NULL, $rid = NULL, $type = NULL) {
  global $user;

  $check_nid = db_result(db_query('SELECT type FROM {node} WHERE nid = %d LIMIT 1', $nid));

  if ($check_nid && $check_nid == 'task') {
    $task = node_load($nid);
    if ($task) {
      $task = (object) array_merge((array) $task, $data);
    }
  }
  else {
    $r_node = node_load($rid);

    if ($r_node) {
      $task = new stdClass();
      $task->type = 'task';
      $task->task_type = $type;
      $task->rid = $r_node->nid;
      $task->task_status = 0;
      $task->uid = $user->uid;
      switch ($type) {
        case 'clone':
          $task->task_args = array(
            'target_platform' => $node->platform,
            'new_db_server' => $node->db_server,
            'new_uri' => $options['clone_url'],
          );
          break;
        default:
          break;
      }
    }
  }

  node_validate($task);

  $task->status=1;
  return node_save($task);
}

/**
 * Get task status
 */
function hosting_services_get_task($nid){
  return node_load(array('nid' => $nid));    
}
