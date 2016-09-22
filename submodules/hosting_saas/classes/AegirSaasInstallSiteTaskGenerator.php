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

}
