<?php

// TODO: Check that the nid is a really of type task...

/**
 * Create a task
 */
function hosting_services_save_task($nid = NULL, $type = NULL, $args = NULL) {

  // Attempt to load the node we're operating on.
  $node = node_load($nid);

  // Bail if a node couldn't be loaded.
  if (!$node) {
    watchdog('hosting_services', 'Cannot create task: Node "%nid" cannot be loaded.', array('%nid' => $nid), WATCHDOG_ERROR);
    return FALSE;
  }

  // Perform action on loaded node.
  $task = hosting_add_task($nid, $type, $args);

  // Return the resulting task.
  return $task;
}

/**
 * Get task status
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
