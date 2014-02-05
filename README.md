Aegir Services
==============

[Aegir Services](http://drupal.org/project/hosting_services) integrates the Services API framework into the Hostmaster suite of tools. It allows Aegir managers the ability to build Services API connections over any of the available servers for Services API.

Using Services 3.x we are aiming at full CRUD support for clients, tasks and sites, as well as read access to profiles, platforms, and eventually, packages.

The 2.x branch of [Aegir Ubercart Integration](http://community.aegirproject.org/node/516) will take advantage of this support to provide a remote storefront functionality.

Please report issues and request support via the [issue queue on Drupal.org](http://drupal.org/project/issues/hosting_services?categories=All)

Installation
------------

1. Install [Aegir](http://community.aegirproject.org/installing)
2. Install the **dev** version of [services 3.x](http://drupal.org/project/services), including at least one server and, ideally, an authentication server.
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

