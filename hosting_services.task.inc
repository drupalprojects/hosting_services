<?php

/**
 * Create a task.
 *
 * @param $nid
 *   The ID of the node to act upon.
 * @param $type
 *   A string representing the task type to create.
 * @param $args
 *   An array with arguments specific to the type of task.
 * @return
 *   The fully loaded task, or FALSE if unable to create.
 */
function hosting_services_save_task($nid = NULL, $type = NULL, $args = NULL) {

  // Bail if we don't have a valid node ID.
  if (!$nid) {
    watchdog('hosting_services', 'Cannot create task: ID of node to act upon was not provided.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Get the node ID if we were given an object.
  if (is_object($nid) && !empty($nid->nid)) {
    $nid = $nid->nid;
  }
  // Bail if the node to act upon couldn't be loaded.
  elseif (!$node = node_load($nid)) {
    watchdog('hosting_services', 'Cannot create task: Node "%nid" cannot be loaded. Either it never existed or was deleted.', array('%nid' => $nid), WATCHDOG_ERROR);
    return FALSE;
  }

  // Perform action on loaded node.
  $task = hosting_add_task($nid, $type, $args);

  // Return the resulting task.
  return $task;
}

/**
 * Get task status
 *
 * @todo Check that the nid is really of type task.
 */
function hosting_services_get_task($nid){
  return node_load($nid);
}

/**
 * Gets tasks and their status for a site, server or platform
 */
function hosting_services_get_task_list($nid) {
  if (node_load($nid)) {
    return hosting_task_fetch_tasks($nid);
  }
}

/**
 * Gets the task log for the given nid
 */
function hosting_services_get_task_log($nid) {
  $node = node_load($nid);

  if ($node) {
    $result = db_query("SELECT * FROM {hosting_task_log} WHERE vid = :vid ORDER BY lid", array(':vid' => $node->vid));

    return $result->fetchAll();
  }
}
