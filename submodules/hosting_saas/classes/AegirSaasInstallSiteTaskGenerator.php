<?php

/**
 * Generates Install site-creation tasks for Aegir SaaS.
 */
class AegirSaasInstallSiteTaskGenerator extends AegirSaasCreateSiteTaskGenerator {

  const ARGUMENT_PLATFORM = 'platform';
  const ARGUMENT_DATABASE = 'db_server';

  /**
   * {@inheritdoc}
   */
  public function setSiteIdAndVerify() {
    if (hosting_get_site_by_url($this->site, FALSE)) {
      $message = "Cannot populate task: Site name matches an existing site. A new name must be provided.";
      $this->logErrorAndThrowException($message);
    }
  }

  protected function getNewSiteName() {
    return $this->site;
  }

  protected function injectConfigurationSettingsNotProvidedByRequest() {
    parent::injectConfigurationSettingsNotProvidedByRequest();

    if (empty($this->arguments['profile'])) {
      if (empty($this->arguments['profile'] = variable_get('hosting_saas_template_profile', ''))) {
        $this->logErrorAndThrowException(t("Cannot populate site Install task: Installation profile ID was not specified. Either save it in the settings, or provide it as 'profile' in the POST form data."));
      }
    }
    else {
      if (!$profile = hosting_get_package($this->arguments['profile'], 'profile')) {
        $this->logErrorAndThrowException(t("Cannot populate site Install task: The specified profile name %profile does not match an existing profile.", array(
          '%profile' => $this->arguments['profile'],
        )));
      }
      $this->arguments['profile'] = $profile->nid;
    }
  }

}
