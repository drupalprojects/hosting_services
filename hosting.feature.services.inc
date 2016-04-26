<?php

/**
 * @file
 *   The Hosting feature definition for Services.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_services_hosting_feature() {
  $features['services'] = array(
    'title' => t('Services'),
    'description' => t('Provides integration with the Services API (3.x).'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_services',
    'group' => 'experimental'
    );
  return $features;
}
