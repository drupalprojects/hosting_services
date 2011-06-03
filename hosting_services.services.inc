<?php 

/* Remove _disable and _enable hooks after development */
function hosting_services_disable(){
	cache_clear_all('services:methods', 'cache');
}

function hosting_services_enable(){
	cache_clear_all('services:methods', 'cache');
}  //*/

/*
 * Implementation of hook_service
 */
function hosting_services_service(){
    return array(
        // Client Services
        array( /* Create the definition to get a Client remotely from Aegir */
    		'#method' => 'client.get',
    		'#callback' => 'hosting_services_client',
    		'#args' => array(
                array( 
    				'#name' => 'email',
    				'#type' => 'string',
    				'#description' => t('Lookup a client on Aegir, returns Client Node on Success or False on fail.')
                ),
            ),
    		'#access callback' => 'get_client_access',
    		'#access arguments' => TRUE,
    		'#return' => 'struct',
    		'#file' => array('file' => 'client.inc', 'module' => 'hosting_services'),
        ),
        array( /* Get a give Client's Sites */
    		'#method' => 'client.sites',
    		'#callback' => 'hostmaster_service_client_sites',
    		'#args' => array( 
                array(
    				'#name' => 'email',	
    				'#type' => 'string', 
    				'#optional' => FALSE,
    				'#description' => t("Returns an Array of the client's sites on Aegir")
                ),
            ),
    		'#access callback' => 'client_site_access',
    		'#file' => array('file' => 'client.inc', 'module' => 'hosting_services'),
		),
		array(
			/* Create a new client */
			'#method' => 'client.save',
			'#callback' => 'hostmaster_service_client_save',
			'#args' => array(
				array(
					'#name' => 'email',
					'#type' => 'string',
					'#optional' => FALSE,
					'#description' => t('Email Address of the new client'),
				),
				array(
					'#name' => 'name',
					'#type' => 'string',
					'#optional' => FALSE,
					'#description' => t('Name of the new client')
				)
			),
			'#access callback' => 'save_client_access',
			'#file' => array('file' => 'client.inc', 'module' => 'hosting_services'),
			'#return' => 'struct'
			
		),
		array(
		    /* Suspend All Sites owned by client */
			'#method' => 'client.suspend_sites',
			'#callback' => 'suspendClientSites',
			'#access arguments' => array('suspend client sites'),
			'#file' => array('file' => 'client.inc', 'module' => 'hosting_services'),
			'#args' => array(
				array(
				'#name' => 'email', 
				'#type' => 'string', 
				'#description' => t('Email of the client to disable sites for.') 
				),
				array(
				'#name' => 'message', 
				'#type' => 'string', 
				'#optional' => TRUE,
				'#description' => t('Reason for Account Suspension')
				)
			)
		),
		array(
		    /* Unsuspend All Sites owned by client */
			'#method' => 'client.unsuspend_sites',
			'#callback' => 'unsuspendClientSites',
			'#access arguments' => array('unsuspend client sites'),
			'#file' => array('file' => 'client.inc', 'module' => 'hosting_services'),
			'#args' => array(
							array('#name' => 'email', '#type' => 'string', '#description' => t('Email of the client to disable sites for.') )
			)
		),
		// Platform Services
		array(
		    /* List Platforms */
			'#method' => 'platform.list',
			'#callback' => 'platformList',
			'#description' => t('Returns the List of Platforms that are available to create new sites on via Services API'),
			'#access arguments' => array('list platforms'),
			'#return' => 'array',
			'#file' => array('file' => 'platform.inc', 'module' => 'hosting_services'),
			'#args' => array(
				array(
					'#name' => 'row_count',
					'#type' => 'int',
					'#description' => t('You can limit the number of returned results to cut down on load time'),
					'#optional' => TRUE
				),
				array(
					'#name' => 'offset',
					'#type' => 'int',
					'#description' => t('Set the offset of rows returned'),
					'#optional' => TRUE 
				)
			)
		
		),
		array(
			'#method' => 'platform.get',
			'#callback' => 'platformGet',
			'#access arguments' => array('get platform'),
			'#description' => t('Get a Platform'),
			'#file' => array('file' => 'platform.inc', 'module' => 'hosting_services'),
			'#args' => array(
				array(
					'#name' => 'nid',
					'#type' => 'int',
					'#optional' => FALSE,
					'#description' => t('Enter the Node ID for the platform you wish to get, Returns a list of available Profiles')
				)
			)
		),
		/*
		 * Profile Services
		 */
		array(
		    // List All profiles
			'#method' => 'profile.list',
			'#callback' => 'profileList',
			'#description' => t('List all Available Profiles'),
			'#access arguments' => array('list profiles'),
			'#file' => array('file' => 'profile.inc', 'module' => 'hosting_services'),
		),
		array(
		    // Get Profiles
			'#method' => 'profile.get',
			'#callback' => 'profileGet',
			'#description' => t('Get a Profile, Returns also a list of valid Platforms'),
			'#access arguments' => array('get profile'),
			'#file' => array('file' => 'profile.inc', 'module' => 'hosting_services'),
			'#args' => array(
				array(
					'#name' => 'nid',
					'#type' => 'int',
					'#optional' => FALSE,
					'#description' => t('Enter the Node ID for the profile you wish to get')
				)
			)
		),
		// Task Services
		array(
			'#method' => 'task.create',
			'#callback' => 'createTask',
			'#description' => t('Create an Aegir task'),
			'#access arguments' => array('create task'),
			'#file' => array('file' => 'task.inc', 'module' => 'hosting_services'),
			'#args' => array(
				array(
					'#name' => 'url',
					'#type' => 'string',
					'#optional' => FALSE,
					'#description' => t('Site URL')
				),
				array(
					'#name' => 'type',
					'#type' => 'string',
					'#optional' => FALSE,
					'#description' => t('Task Type')
				),
				array(
					'#name' => 'options',
					'#optional' => TRUE,
					'#type' => 'array',
					'#description' => t('Additional Task options')
				)
			)
		),
		array(
			'#method' => 'task.get',
			'#callback' => 'getTask',
			'#description' => t('Get a Task node'),
			'#file' => array('file' => 'task.inc', 'module' => 'hosting_services'),
			'#access arguments' => array('get task'),
			'#args' => array(
				array( '#name' => 'nid', '#type' => 'int', '#description' => t('Node id of the task'))
			)
		),
		// Site services
		array(
			'#method' => 'site.save',
			'#callback' => 'saveSite',
			'#description' => t('Site Save'),
			'#access arguments' => array('save site'),
			'#file' => array('file' => 'site.inc', 'module' => 'hosting_services'),
			'#args' => array(
				array( '#name' => 'site', '#type' => 'struct', '#description' => t('URL')),
			)
		),
		array(
			'#method' => 'site.get',
			'#callback' => 'getSite',
			'#description' => t('Get Site'),
			'#access arguments' => array('get site'),
			'#file' => array('file' => 'site.inc', 'module' => 'hosting_services'),
			'#args' => array(
				array( '#name' => 'url', '#type' => 'string', '#description' => t('URL')),
			)
		)
	);
}

?>
