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
  $platform_query = db_query('SELECT n.nid, n.title FROM {node} n INNER JOIN {hosting_services_platforms} hsp ON hsp.nid = n.nid INNER JOIN {hosting_platform} hp ON hp.nid = n.nid WHERE n.type = "platform" AND hp.status = %d' . $conditions, HOSTING_PLATFORM_ENABLED);
  while($plat = db_fetch_array($platform_query)){
    $platforms[] = $plat;
  }
  return $platforms;
}
 
/**
 * Returns a specific platform
 */
function hosting_services_platform_get($nid){
  $platform = node_load(array('type'=>'platform', 'nid' => $nid)); 
  return $platform;
}
