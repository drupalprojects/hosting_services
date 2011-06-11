<?php 

/**
 * Enables Remote Accessible platform listing
 */
function hosting_services_platform_list($rows = 0, $offset = 0){
  
  $platform_query = db_query('SELECT * FROM {node} WHERE type = "platform" ');
  while($plat = db_fetch_object($platform_query)){
    $platform = node_load(array('nid' => $plat->nid) );
    if(!_hosting_platform_is_locked($platform) && $platform->services_enabled == 1){
      $platforms[] = $platform;
    }
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
