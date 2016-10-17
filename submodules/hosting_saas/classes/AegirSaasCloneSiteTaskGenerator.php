<?php

/**
 * Generates Clone site-creation tasks for Aegir SaaS.
 */
class AegirSaasCloneSiteTaskGenerator extends AegirSaasCreateSiteTaskGenerator {

  const ARGUMENT_PLATFORM = 'target_platform';
  const ARGUMENT_DATABASE = 'new_db_server';

  /**
   * {@inheritdoc}
   *
   * It's not a problem if the ID getter fails here because we'll later attempt
   * to get the ID from the configuration.  If that fails too, we'll complain
   * about it over there.
   */
  public function setSiteIdAndVerify() {
    $this->site = hosting_get_site_by_url($this->site, FALSE);
  }

  protected function getNewSiteName() {
    return $this->arguments['new_uri'];
  }

  protected function injectConfigurationSettingsNotProvidedByRequest() {
    parent::injectConfigurationSettingsNotProvidedByRequest();

    if (empty($this->site)) {
      if (empty($template_site_configured = variable_get('hosting_saas_template_site_nid', ''))) {
        $this->logErrorAndThrowException(t("Cannot populate site clone task: Template site name was not specified. Either save it in the settings, or provide it as 'nid' in the POST form data."));
      }
      if (!$this->site = hosting_get_site_by_url($template_site_configured, FALSE)) {
        $this->logErrorAndThrowException(t("Cannot populate site clone task: The saved template site name %template does not match an existing site.", array(
          '%template' => $template_site_configured,
        )));
      }
    }

    $profiles_available = hosting_get_profiles($this->getTargetPlatform());
    $site = node_load($this->site);
    if (!in_array($site->profile, array_keys($profiles_available))) {
      $this->logErrorAndThrowException(t("Cannot populate site Clone task: The requested platform %platform does not contain the profile %profile necessary for cloning site %site.", array(
        '%platform' => node_load($this->getTargetPlatform())->title,
        '%profile' => node_load($site->profile)->title,
        '%site' => $site->title,
      )));
    }
  }

}
