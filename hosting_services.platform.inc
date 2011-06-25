<?php 

/**
 * Enables Remote Accessible platform listing
 *
 * TODO Make use of the $rows and $offset parameters
 */
function hosting_services_platform_list($rows = 0, $offset = 0){

  $platform_query = db_query('SELECT nid FROM {node} WHERE type = "platform" ');
  while($plat = db_fetch_object($platform_query)){
    $platform = node_load($plat->nid);
    die (var_export($platform, true));
    if(!_hosting_platform_is_locked($platform) && $platform->remote_site_creation_enabled == 1){
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
