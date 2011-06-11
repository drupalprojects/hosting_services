<?php 
/**
 * Creates a task and returns it's node id if successful
 */
function hosting_services_create_task($sess, $url, $type, $option = array() ){
  global $user;
  $node = node_load(array('title' => $url ));
  watchdog('Hostmaster Debug', 'node id => '.$node->nid.' Remote url: '.$url.' Type '.$type);
  // $task = fengafim_arrayToObject($remote);
  switch($type){
    default:
      $task = new stdClass();
      $task->type = 'task';
      $task->task_type = $type;
      $task->rid = $node->nid;
      $task->task_status = 0;
      $task->uid = $user->uid;
      return node_save($task);
    break;
    case 'clone':
      $task = new stdClass();
      $task->type = 'task';
      $task->task_type = $type;
      $task->rid = $node->nid;
      $task->task_status = 0;
      $task->uid = $user->uid;
      $task->task_args = array( 
        'target_platform' => $node->platform,
        'new_db_server' => $node->db_server,
        'new_uri' => $option[0]
      );
      return node_save($task);
    break;
  }
}

/**
 * Get task status
 */
function hosting_services_get_task($nid){
  return node_load(array('nid' => $nid));    
}
