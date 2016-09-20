<?php

/**
 * Generates site creation tasks for Aegir SaaS.
 */
class AegirSaasCreateSiteTaskGenerator extends AegirSaasSiteTaskGenerator {

  public function prepareForSiteCreation() {
    $this->saveVariablesForInjection();
    $this->prepareClientUserInfo();
    $this->injectConfigurationSettingsNotProvidedByRequest();
  }

  protected function saveVariablesForInjection() {
    $bridged_values = variable_get('hosting_saas_utils_bridged_values', array());
    $variables = variable_get('hosting_saas_utils_variables', array());

    $site_variables = array();
    foreach ($variables as $values) {
      if (!empty($this->arguments[$values['key']])) {
        $site_variables[$values['name']] = $this->arguments[$values['key']];
      }
    }

    if (!empty($site_variables)) {
      $bridged_values[$this->arguments['new_uri']] = $site_variables;
      variable_set('hosting_saas_utils_bridged_values', $bridged_values);
    }
  }

  protected function prepareClientUserInfo() {
    $uri = $this->arguments['new_uri'];
    $todo = variable_get('hosting_saas_todo', array());
    $client_user_email_key = variable_get('hosting_saas_utils_user_email', '');
    $client_user_name_key = variable_get('hosting_saas_utils_user_name', '');

    if (!empty($client_user_email_key) && !empty($this->arguments[$client_user_email_key])) {
      if (!empty($client_user_name_key)) {
        $client_user = array(
          'name' => $this->arguments[$client_user_name_key],
          'email' => $this->arguments[$client_user_email_key],
        );
      }
      else {
        $client_user = array(
          'name' => $this->arguments[$client_user_email_key],
          'email' => $this->arguments[$client_user_email_key],
        );
      }

      if (variable_get('hosting_saas_utils_create_new_user', 1)) {
        $todo[$uri]['create_new_user'] = $client_user;
      }
      else {
        $todo[$uri]['change_admin_user'] = $client_user;
      }

      $role_name = variable_get('hosting_saas_utils_user_role', '');
      if (!empty($role_name)) {
        $todo[$uri]['set_user_role'] = $role_name;
      }

      $send_email = variable_get('hosting_saas_utils_send_email', FALSE);
      if (!empty($send_email)) {
        $todo[$uri]['send_email'] = TRUE;
      }
    }

    variable_set('hosting_saas_todo', $todo);
  }

  protected function injectConfigurationSettingsNotProvidedByRequest() {
    if (empty($this->arguments['new_uri'])) {
      $this->logErrorAndThrowException("Cannot populate site clone task: URL of the new site was not specified. It must be provided in the POST form data as 'options[new_uri]'.");
    }

    // Set empty arguments so that we may reference them.
    if (empty($this->arguments['new_db_server'])) {
      $this->arguments['new_db_server'] = '';
    }
    if (empty($this->arguments['target_platform'])) {
      $this->arguments['target_platform'] = '';
    }

    $template_site = &$this->site;
    $new_site = $this->arguments['new_uri'];
    $database = &$this->arguments['new_db_server'];
    $platform = &$this->arguments['target_platform'];

    if (!$this->siteNameMatchesDomain()) {
      $this->logErrorAndThrowException(t("Cannot populate site creation task: The requested URL '%url' does not match the configured domain '%domain'.", array(
        '%url' => $new_site,
        '%domain' => variable_get('hosting_saas_master_domain', 'localhost'),
      )));
    }

    if (!hosting_domain_allowed($new_site)) {
      $this->logErrorAndThrowException(t('The new site URL "%url" is not allowed. It is likely already being used.', array(
        '%url' => $new_site,
      )));
    }

    if (empty($template_site)) {
      if (empty($template_site_configured = variable_get('hosting_saas_template_site_nid', ''))) {
        $this->logErrorAndThrowException(t("Cannot populate site clone task: Template site name was not specified, or does not match an existing site. Either save it in the settings, or provide it as 'nid' in the POST form data."));
      }
      if (!$template_site = hosting_get_site_by_url($template_site_configured, FALSE)) {
        $this->logErrorAndThrowException(t("Cannot populate site clone task: The saved template site name %template does not match an existing site.", array(
          '%template' => $template_site_configured,
        )));
      }
    }

    if (empty($database)) {
      if (empty($database_configured = variable_get('hosting_saas_db_server', ''))) {
        $this->logErrorAndThrowException(t("Cannot populate site creation task: Target DB server ID not specified. Either save it in the settings, or provide the server node ID as options[new_db_server] in the POST form data."));
      }
      $database = $database_configured;
    }

    if (empty($platform)) {
      if (empty($platform_configured = variable_get('hosting_saas_target_platform', ''))) {
        $this->logErrorAndThrowException(t("Cannot populate site creation task: Target platform ID not specified. Either save it in the settings, or provide the server node ID as options[target_platform] in the POST form data."));
      }
      $platform = $platform_configured;
    }

    if ($this->maximumNumberOfSitesReached() === TRUE) {
      $this->logErrorAndThrowException(t("Cannot fulfill site creation task: The maximum number of sites have already been provisioned through this service."), WATCHDOG_ALERT);
    }
  }

  protected function siteNameMatchesDomain() {
    $site_name = $this->arguments['new_uri'];

    if (empty($domain = variable_get('hosting_saas_master_domain', 'localhost'))) {
      return TRUE;
    }

    $dot_domain = '.' . $domain;
    $domain_length_with_dot = strlen($domain) + 1;

    if (substr_compare($site_name, $dot_domain, -$domain_length_with_dot, $domain_length_with_dot) === 0) {
      return TRUE;
    }
    return FALSE;
  }

  protected function maximumNumberOfSitesReached() {
    $platform = $this->arguments['target_platform'];
    $capacity = variable_get('hosting_saas_max_capacity', '0');

    // If there's no limit, we can never be at capacity.
    if (intval($capacity) <= 0) {
      return FALSE;
    }

    // Fetch the number of sites on the platform.
    $count = hosting_site_count($platform);

    // If the number of sites has reached the limit, report a problem.
    if ($count >= $capacity) {
      return TRUE;
    }

    // Otherwise, everything is fine. Leave the flag down with log entry.
    watchdog('hosting_saas', 'Site count / capacity: %count / %capacity', array(
      '%count' => $count,
      '%capacity' => $capacity,
    ));
    return FALSE;
  }

}
