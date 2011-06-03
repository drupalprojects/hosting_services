<?php 
/*
 * Returns the list of profiles
 */
function profileList(){
	return hosting_get_profiles();
}

/*
 * Returns a single Platform
 */
function profileGet($profileID){
	return node_load(array('nid' => $profileID, 'type' => 'package'));
}

?>