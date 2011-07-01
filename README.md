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
