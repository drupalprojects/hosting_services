<?php

/**
 * @file
 * Aegir Services hooks.
 *
 * This file provides documentation and examples for hooking into Aegir Services
 * via its hooks.
 */

/**
 * Implements hook_hosting_services_site_presave_alter().
 *
 * Alter new and updated sites before they're saved.
 *
 * @param $site
 *   The site node to alter.
 *
 * @see hosting_saas_hosting_services_site_presave_alter()
 */
function hook_hosting_services_site_presave_alter(&$site) {
  $current_user_is_saas = ($GLOBALS['user']->name == HOSTING_SAAS_USER);
  $site_is_new = !isset($site->nid);

  $saas_encryption_state = variable_get('hosting_saas_https_encryption', HOSTING_HTTPS_DISABLED);
  $saas_encryption_requested = ($saas_encryption_state != HOSTING_HTTPS_DISABLED);

  if (module_exists('hosting_https') && hosting_https_is_available($site, FALSE)
      && $current_user_is_saas && $saas_encryption_requested && $site_is_new) {
    $site->https_enabled = $saas_encryption_state;
  }
}
