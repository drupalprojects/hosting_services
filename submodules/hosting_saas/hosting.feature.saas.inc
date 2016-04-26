<?php


/**
 * @file
 *   The hosting feature definition for SaaS
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return
 *  associative array indexed by feature key.
 */
function hosting_saas_hosting_feature() {
  $features['saas'] = array(
    'title' => t('SaaS'),
    'description' => t('Retains software-as-a-service settings.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_saas',
    'group' => 'experimental'
    );
  return $features;
}
