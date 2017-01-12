Aegir SaaS
==========

This module sets up a fully functional endpoint (via the [Aegir Services API](https://www.drupal.org/project/hosting_services)) allowing for remote administration of sites, notably installing new sites and cloning existing sites.  It provides common parameters for site creation as configured in the module's settings.  Using the API's task resource, sites can also be disabled, enabled, deleted, and have any other task performed on them supported by your [Aegir](https://www.drupal.org/project/hostmaster) installation.

## What this module does

1. It creates the *aegir web services* role.
2. It creates the *Aegir SaaS* user (placed in the above role).
3. It adds necessary permissions for the user to issue remote commands (getting site information and running tasks).
4. It configures the new *aegir/saas* [Services](https://www.drupal.org/project/services) endpoint (over at http://example.com/aegir/saas for example).
5. It associates remote commands on the endpoint with the new user.
6. It sets up [API-key-based authentication](https://www.drupal.org/project/services_api_key_auth) on the endpoint.
7. It enables the necessary resources required.
8. It allows you to run tasks on sites with their site names; you don't need their node IDs.

The primary feature of this module is its ability to create new sites with your desired settings.

The endpoint uses API-key authentication by default, but **it will NOT be usable** (the key will change randomly) until you save its configuration.  This is a security feature that prevents the endpoint from being active until you explicitly enable it.

## Dependencies / required modules

* [Services](https://www.drupal.org/project/services)
* [Aegir Services](https://www.drupal.org/project/hosting_services)
* [Services API Key Authentication](https://www.drupal.org/project/services_api_key_auth)
* [Hosting Variables](https://www.drupal.org/project/hosting_variables)

## Installation and set-up

1. Become the Aegir user on your Aegir server.
    * sudo -sHu aegir
2. Download the necessary modules.
    * drush @hm dl hosting_services services services_api_key_auth hosting_variables --destination=$(drush dd @hm:%site)/modules/contrib
3. Enable the Aegir SaaS module.
    * drush @hm en hosting_saas
4. Save your new automatically generated API key.
    * Surf to Administration » Structure » Services.
    * Click on the *Edit Authentication* item in the Operations pull-down menu of *hosting_saas*.
    * Hit the *Save* button.
5. Configure your settings for creating new sites.
    * Surf to Administration » Hosting » SaaS.
    * Enter/change the *Basic settings* form, and then save it.
    * Enter/change the *Site handovers* form, and then save it.
    * Enter/change the *Injected variables* form, and then save it.

For site creation tasks, some arguments do not need to be provided on the main settings form; they can be provided in remote requests.  Request parameters will override configured values in the settings.

## Client usage

### Example

You can test your endpoint with [cURL](https://en.wikipedia.org/wiki/CURL) on the command line:

    curl --data "api-key=your-api-key&type=clone&options[new_uri]=mynewsite.com&target=&options[testing]=test" http://example.com/aegir/saas/task

As you can see, you need to specify the *target* parameter for Services to accept the request, but it can be empty if you want it to be overriden with the default site *target* in the SaaS settings. (*target* is the site to clone, and is entered as the site's name. We're calling it *target* instead of *site* because we may want to support platforms or other target types later.)

If there are errors, you should receive an empty XML response. Errors related to settings will appear in the Recent Logs report (if you have the Database Logging module enabled; it is not enabled by default on Aegir).

### Service call details

#### Using GET

##### List all sites

* http://aegir.example.com/aegir/saas/site.json?api-key=super-secret-random-key

##### Get information on a particular site

* http://aegir.example.com/aegir/saas/site/aegir.example.com.json?api-key=super-secret-random-key

#### Using POST

##### Create site Clone task

* http://aegir.example.com/aegir/saas/task?api-key=super-secret-random-key
* Form data:
    * **target**: *template.example.com* (optional if in settings)
    * **type**: *clone*
    * **options[new_uri]**: *client1.example.com*
    * **options[database]**: *12* (DB server node ID, if not in settings)
    * **options[platform]**: *24* (Platform node ID, if not in settings)
    * **options[client_email]**: *jane.doe@example.com* (if set up in *Site handovers* configuration)
    * **options[client_name]**: *jane.doe* (if set up in *Site handovers* configuration)
    * **options[...]**: (arguments set in your *Injected variables* configuration)

##### Create site Install task

* http://aegir.example.com/aegir/saas/task?api-key=super-secret-random-key
* Form data:
    * **target**: *client1.example.com*
    * **type**: *install*
    * **options[profile]**: *standard* (installation profile short name / ID, if not in settings)
    * **options[database]**: *12* (DB server node ID, if not in settings)
    * **options[platform]**: *24* (Platform node ID, if not in settings)
    * **options[client_email]**: *jane.doe@example.com* (if set up in *Site handovers* configuration)
    * **options[client_name]**: *jane.doe* (if set up in *Site handovers* configuration)
    * **options[...]**: (arguments set in your *Injected variables* configuration)

##### Create site Disable task

* http://aegir.example.com/aegir/saas/task?api-key=super-secret-random-key
* Form data:
    * **target**: *client1.example.com*
    * **type**: disable

##### Create site Enable task

* http://aegir.example.com/aegir/saas/task?api-key=super-secret-random-key
* Form data:
    * **target**: *client1.example.com*
    * **type**: enable

##### Create site Delete task

* http://aegir.example.com/aegir/saas/task?api-key=super-secret-random-key
* Form data:
    * **target**: *client1.example.com*
    * **type**: delete
