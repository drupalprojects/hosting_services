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

}
