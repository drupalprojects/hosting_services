<?php

/**
 * Generates site tasks for Aegir SaaS.
 */
class AegirSaasSiteTaskGenerator {

  /**
   * Map task types to generator classes.
   */
  const MAP = [
    'clone' => 'AegirSaasCloneSiteTaskGenerator',
    'install' => 'AegirSaasInstallSiteTaskGenerator',
  ];

  protected $site;
  protected $task_type;
  protected $arguments;

  public function __construct($site_name, $task_type, $arguments) {
    $this->site = $site_name;
    $this->task_type = $task_type;
    $this->arguments = $arguments;
  }

  public function setSiteIdAndVerify() {
    if (!$this->site = hosting_get_site_by_url($this->site, FALSE)) {
      $message = "Cannot populate task: Site name was not specified, or does not match an existing site. It must be provided in the POST form data as 'nid'.";
      $this->logErrorAndThrowException($message);
    }
  }

  protected function logErrorAndThrowException($message, $severity = WATCHDOG_ERROR) {
    watchdog('hosting_saas', $message, array(), $severity);
    throw new Exception($message);
  }
}
