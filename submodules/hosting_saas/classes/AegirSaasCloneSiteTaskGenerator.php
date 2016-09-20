<?php

/**
 * Generates Clone site-creation tasks for Aegir SaaS.
 */
class AegirSaasCloneSiteTaskGenerator extends AegirSaasCreateSiteTaskGenerator {

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

  protected function newSiteName() {
    return $this->arguments['new_uri'];
  }

  protected function injectConfigurationSettingsNotProvidedByRequest() {
    parent::injectConfigurationSettingsNotProvidedByRequest();

    if (empty($this->site)) {
      if (empty($template_site_configured = variable_get('hosting_saas_template_site_nid', ''))) {
        $this->logErrorAndThrowException(t("Cannot populate site clone task: Template site name was not specified, or does not match an existing site. Either save it in the settings, or provide it as 'nid' in the POST form data."));
      }
      if (!$this->site = hosting_get_site_by_url($template_site_configured, FALSE)) {
        $this->logErrorAndThrowException(t("Cannot populate site clone task: The saved template site name %template does not match an existing site.", array(
          '%template' => $template_site_configured,
        )));
      }
    }
  }

}