<?php


/**
 * @file
 *   The Hosting feature definition for SaaS.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_saas_hosting_feature() {
  $features['saas'] = array(
    'title' => t('SaaS'),
    'description' => t('Retains software-as-a-service settings for site-factory set-ups.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_saas',
    'group' => 'experimental'
    );
  return $features;
}
