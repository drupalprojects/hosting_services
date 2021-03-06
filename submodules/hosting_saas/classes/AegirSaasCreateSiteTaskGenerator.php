<?php

/**
 * Generates site creation tasks for Aegir SaaS.
 */
class AegirSaasCreateSiteTaskGenerator extends AegirSaasSiteTaskGenerator {

  const ARGUMENT_PLATFORM = 'platform';
  const ARGUMENT_DATABASE = 'database';

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
      $bridged_values[$this->getNewSiteName()] = $site_variables;
      variable_set('hosting_saas_utils_bridged_values', $bridged_values);
    }
  }

  protected function prepareClientUserInfo() {
    $uri = $this->getNewSiteName();
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
    if (empty($this->getNewSiteName())) {
      $this->logErrorAndThrowException("Cannot populate site creation task: URL of the new site was not specified. It must be provided in the POST form data as 'options[new_uri]' (for Clone) or 'nid' (for Install).");
    }

    if (!$this->siteNameMatchesDomain()) {
      $this->logErrorAndThrowException(t("Cannot populate site creation task: The requested URL '%url' does not match the configured domain '%domain'.", array(
        '%url' => $this->getNewSiteName(),
        '%domain' => variable_get('hosting_saas_master_domain', 'localhost'),
      )));
    }

    if (!hosting_domain_allowed($this->getNewSiteName())) {
      $this->logErrorAndThrowException(t('The new site URL "%url" is not allowed. It is likely already being used.', array(
        '%url' => $this->getNewSiteName(),
      )));
    }

    if (empty($this->getTargetDatabase())) {
      if (empty($database_configured = variable_get('hosting_saas_db_server', ''))) {
        $this->logErrorAndThrowException(t("Cannot populate site creation task: Target DB server ID not specified. Either save it in the settings, or provide the server node ID as options[database] in the POST form data."));
      }
      $this->setTargetDatabase($database_configured);
    }

    if (empty($this->getTargetPlatform())) {
      if (empty($platform_configured = variable_get('hosting_saas_target_platform', ''))) {
        $this->logErrorAndThrowException(t("Cannot populate site creation task: Target platform ID not specified. Either save it in the settings, or provide the server node ID as options[platform] in the POST form data."));
      }
      $this->setTargetPlatform($platform_configured);
    }

    if ($this->maximumNumberOfSitesReached() === TRUE) {
      $this->logErrorAndThrowException(t("Cannot fulfill site creation task: The maximum number of sites have already been provisioned through this service."), WATCHDOG_ALERT);
    }
  }

  protected function siteNameMatchesDomain() {
    if (empty($domain = variable_get('hosting_saas_master_domain', 'localhost'))) {
      return TRUE;
    }

    $dot_domain = '.' . $domain;
    $domain_length_with_dot = strlen($domain) + 1;

    if (substr_compare($this->getNewSiteName(), $dot_domain, -$domain_length_with_dot, $domain_length_with_dot) === 0) {
      return TRUE;
    }
    return FALSE;
  }

  protected function maximumNumberOfSitesReached() {
    $platform = $this->getTargetPlatform();
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

  protected function getPlatformArgument($parent = FALSE) {
    return $parent ? self::ARGUMENT_PLATFORM : static::ARGUMENT_PLATFORM;
  }

  protected function getDatabaseArgument($parent = FALSE) {
    return $parent ? self::ARGUMENT_DATABASE : static::ARGUMENT_DATABASE;
  }

  protected function getTargetPlatform() {
    return isset($this->arguments[$this->getPlatformArgument()]) ? $this->arguments[$this->getPlatformArgument()] : '';
  }

  protected function getTargetDatabase() {
    return isset($this->arguments[$this->getDatabaseArgument()]) ? $this->arguments[$this->getDatabaseArgument()] : '';
  }

  protected function setTargetPlatform($platform) {
    // Set both generic and specific arguments are they're used in different places.
    $this->arguments[$this->getPlatformArgument()] = $platform;
    $this->arguments[$this->getPlatformArgument(TRUE)] = $platform;
  }

  protected function setTargetDatabase($database) {
    // Set both generic and specific arguments are they're used in different places.
    $this->arguments[$this->getDatabaseArgument()] = $database;
    $this->arguments[$this->getDatabaseArgument(TRUE)] = $database;
  }

}
