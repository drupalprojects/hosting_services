<?php
/**
 * @file
 * Provide remote access to the signup module
 */

/**
 * Return the signup form
 */
function hosting_services_signup_form() {
  if (module_exists('hosting_signup')) {
    $form = drupal_get_form('hosting_signup_form');
    return $form;
  }
  else {
    return t('The signup form module is not enabled.');
  }
}
