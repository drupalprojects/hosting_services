Aegir Services
==============

[Aegir Services](http://drupal.org/project/hosting_services) integrates the Services API framework into the Hostmaster suite of tools. It allows Aegir managers the ability to build Services API connections over any of the available servers for Services API.

Using Services 3.x we are aiming at full CRUD support for clients, tasks and sites, as well as read access to profiles, platforms, and eventually, packages.

Modules like [Aegir Ubercart Integration](https://www.drupal.org/project/uc_hosting) and [Aegir Commerce Integration](https://www.drupal.org/project/commerce_hosting) will take advantage of this support to provide a remote storefront functionality.

Please report issues and request support via the [issue queue on Drupal.org](http://drupal.org/project/issues/hosting_services?categories=All)

Installation
------------

1. Install [Aegir](https://aegir.readthedocs.org/en/3.x/install/)
2. Install [services 3.x](https://www.drupal.org/project/services), including at least one server and, ideally, an authentication server.
3. Install this module.
4. [Configure your endpoint, it's resources and authentication, normally](http://drupal.org/node/736522).
5. Optionally, test your rest server using the included bash script, `hosting_services.rest_test.sh`


Examples
------------

1. OAuth Call to list sites

//Your Application key and secret
$conskey = 'XXXXXXX';
$conssec = 'XXXXXXXX';

//Your Application URL
$api_url = 'http://aegir-server.local/aegir/hosting_site';

try {
  $oauth=new OAuth($conskey,$conssec,OAUTH_SIG_METHOD_HMACSHA1,OAUTH_AUTH_TYPE_URI);
  $oauth->enableDebug();

  $oauth->fetch("$api_url.json");
  $sites = json_decode($oauth->getLastResponse());
  return $sites;

} catch(OAuthException $E) {
  return $E;
}

2. OAuth Call to enable a specific site

$conskey = 'dAnm6VqYz6gCUHzF7ncEntfBSLFUcSvY';
$conssec = 'u8Lz3nukojWkJGuC4cgQuqvQA26rGpxB';

$consumer = new OAuthConsumer($conskey, $conssec, NULL);
$api_url = 'http://aegir3-dev.aegir3.local/aegir/hosting_site/enable';
$params = array('type' => 'enable');
$request = OAuthRequest::from_consumer_and_token($consumer, NULL, 'POST', $api_url, $params);
$request->sign_request(new OAuthSignatureMethod_HMAC_SHA1(), $consumer, NULL);
$p = array('nid' => 'XXX');
$data = http_build_query($p, '', '&');
$options = array(
  'headers' => array(
    'Accept' => 'application/json',
  ),
  'method' => 'POST',
  'data' => $data
);

$response = drupal_http_request($request->to_url(), $options);

if($response->code == 200){
  $enable_task = json_decode($response->data);
}

3. For many use cases, creating sites should be done via the client node and
   client user. The use case is"
   -  Set user accounts to be created when a client node is created.
   -  Then add the below snippet to a custom module.
   -  When making a create client call to your aegir install, the created client
      node is created and returned with an account property containing the user
      that was created including the oauth credentials.

/**
 * Implements hook_user_insert().
 */
function mymodule_user_insert(&$edit, $account, $category){
  $consumer = new DrupalOAuthConsumer(user_password(32), user_password(32), array(
    'callback_url' => 'example.com',
    'uid' => $account->uid,
    'name' => $account->mail,
    'context' => 'aegir_authentication',
    'provider_consumer' => TRUE,
  ));
  $consumer->write();

}

/**
 * Implements hook_user_load().
 */
function mymodule_user_load($users) {
  module_load_include('inc', 'oauth_common');
  foreach ($users as $uid => $user) {
    $ci = oauth_common_user_consumers($user->uid);
    $users[$uid]->oauth = $ci;
  }
}
