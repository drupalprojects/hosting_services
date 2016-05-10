Aegir SaaS
==========

This module allows a simplified workflow.

It creates a default Services endpoint at aegir/saas (http://example.com/aegir/saas).

The endpoint uses API key authentication by default, but **it will NOT be usable** until you go to the form and press "Save" in the Services Authentication page for the ressource: http://example.com/admin/structure/services/list/hosting_saas/authentication

This module creates an Aegir SaaS user and role that have the right permissions for the creation by default.

The task type to use is clone. You can specify the settings in the request or in the settings (Hosting -> Saas). If you have both, the request's parameters will override the form's settings.

You can test your endpoint with curl on the command line:

    curl --data "api-key=your-api-key&type=clone&options[new_uri]=mynewsite.com&nid=&options[testing]=test" http://example.com/aegir/saas/task

As you can see, you need to specify the nid parameter for Services to accept the request, but it can be empty if you want it to be overriden with the default site nid in the SaaS settings. (nid is the site to clone.)

If there are errors, you should receive an empty XML response. Errors related to settings will appear in the Recent Logs report (if you have the Database Logging module enabled, it is not enabled by default on Aegir).
