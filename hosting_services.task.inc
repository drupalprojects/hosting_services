<?php

/**
 * Create a task.
 *
 * @param $target
 *   The task target: A node ID, node object or a site name.
 * @param $type
 *   A string representing the task type to create.
 * @param $args
 *   An array with arguments specific to the type of task.
 * @return
 *   The fully loaded task or site (for new site installs).
 *   FALSE if unable to create.
 */
function hosting_services_save_task($target = NULL, $type = NULL, $args = NULL) {

  if (!$target) {
    watchdog('hosting_services', 'Cannot create task: target was not provided.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  if ($type == 'install') {
    module_load_include('inc', 'hosting_services', 'hosting_services.site');
    return hosting_services_create_site($target, $args);
  }

  // Get the node ID if we were given an object.
  if (is_object($target) && !empty($target->nid)) {
    $target = $target->nid;
  }
  // Bail if the node to act upon couldn't be loaded.
  elseif (!$node = node_load($target)) {
    watchdog('hosting_services', 'Cannot create task: Node "%nid" cannot be loaded. Either it never existed or was deleted.', array('%nid' => $target), WATCHDOG_ERROR);
    return FALSE;
  }

  $task = hosting_add_task($target, $type, $args);
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
