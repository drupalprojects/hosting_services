<?php

/**
 * @file
 * Include file for additional SaaS configuration forms and processing.
 *
 * Forms and processing for everything outside of the basic settings (the
 * settings landing page) goes into this file.
 */

/**
 * Configuration form for SaaS client-site handovers.
 */
function hosting_saas_settings_clients($form, &$form_state) {

  $form['hosting_saas_utils_create_new_user'] = array(
    '#type' => 'radios',
    '#title' => t('Initial client user of newly-created sites'),
    '#default_value' => variable_get('hosting_saas_utils_create_new_user', 1),
    '#options' => array(
      1 => t('New user (Recommended)'),
      0 => t('Default "admin" user (Security warning: Always has all permissions and full site control)'),
    ),
    '#description' => t('The first client user to set up on the new site.'),
  );

  $form['hosting_saas_utils_user_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial client user e-mail variable'),
    '#description' => t("Enter the variable name that will contain the initial client user's e-mail address. Leave empty to not create or edit a user."),
    '#default_value' => variable_get('hosting_saas_utils_user_email', ''),
    '#attributes' => array(
      'placeholder' => 'client_email',
    ),
    '#required' => FALSE,
  );

  $form['hosting_saas_utils_user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial client user name variable'),
    '#description' => t("Enter the variable name that will contain the initial client user's name. Will default to the e-mail variable's value."),
    '#default_value' => variable_get('hosting_saas_utils_user_name', ''),
    '#attributes' => array(
      'placeholder' => 'client_name',
    ),
    '#required' => FALSE,
  );

  $form['hosting_saas_utils_user_role'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial client user role'),
    '#description' => t('Enter the name or ID of the role to assign to the initial client user. This must be the ID on Drupal 8 sites until <a href="https://github.com/drush-ops/drush/issues/2337">user-add-role fails on Drupal 8 when role label specified</a> is fixed in your Drush installation. Leave empty for none.'),
    '#default_value' => variable_get('hosting_saas_utils_user_role', NULL),
    '#attributes' => array(
      'placeholder' => 'Owner',
    ),
    '#required' => FALSE,
  );

  $form['hosting_saas_utils_send_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send initial client user a registration e-mail'),
    '#description' => t('This will use the default user registration message. You can set the message in your template site or enter it in the Variables tab of the site node.'),
    '#default_value' => variable_get('hosting_saas_utils_send_email', 1),
  );

  return system_settings_form($form);
}

/**
 * Configuration form for SaaS Variables.
 *
 * @see hosting_saas_settings_variables_submit
 */
function hosting_saas_settings_variables($form, &$form_state) {

  $form['hosting_saas_utils_variables_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Variables'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t("Values to assign to site variables based on POST variables. This allows a 'bridge' from your subscription form to a site's variable. Exemple: whateveryouwant => site_name."),
    '#prefix' => '<div id="hosting-saas-utils-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  if (!isset($form_state['hosting_saas_utils_variables'])) {
    $variables = variable_get('hosting_saas_utils_variables', array());
    foreach ($variables as $values) {
      $form_state['hosting_saas_utils_variables'][] = array('key' => $values['key'], 'name' => $values['name']);
    }

    $form_state['hosting_saas_utils_variables'][] = array('key' => '', 'name' => '');
  }
  else {
    foreach ($form_state['hosting_saas_utils_variables'] as $counter => $variable) {
      if (isset($form_state['values']['hosting_saas_utils_variable_key_' . $counter])) {
        if ($form_state['hosting_saas_utils_variables'][$counter]['key'] <> $form_state['values']['hosting_saas_utils_variable_key_' . $counter] || $form_state['hosting_saas_utils_variables'][$counter]['name'] <> $form_state['values']['hosting_saas_utils_variable_name_' . $counter]) {
          $form_state['hosting_saas_utils_variables'][$counter]['key'] = $form_state['values']['hosting_saas_utils_variable_key_' . $counter];
        }
        if ($form_state['hosting_saas_utils_variables'][$counter]['name'] <> $form_state['values']['hosting_saas_utils_variable_name_' . $counter]) {
          $form_state['hosting_saas_utils_variables'][$counter]['name'] = $form_state['values']['hosting_saas_utils_variable_name_' . $counter];
        }

      }
    }
  }

  foreach ($form_state['hosting_saas_utils_variables'] as $counter => $variable) {
    _hosting_saas_utils_append_variable_fieldset($form, $counter, $variable['key'], $variable['name']);
  }

  _hosting_saas_utils_append_variable_fieldset($form, $counter);

  $form['hosting_saas_utils_variables_fieldset']['add_variable'] = array(
    '#type' => 'submit',
    '#value' => t('Add more'),
    '#submit' => array('hosting_saas_utils_add_variable'),
    '#ajax' => array(
      'callback' => 'hosting_saas_utils_ajax_callback',
      'wrapper' => 'hosting-saas-utils-fieldset-wrapper',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  // Can't use system_settings_form() because of the dynamic variables array.
  return $form;
}

/**
 * Submission handler for the SaaS Variables configuration form.
 *
 * @see hosting_saas_settings_variables()
 */
function hosting_saas_settings_variables_submit(&$form, &$form_state) {
  $variables = array();
  foreach ($form_state['hosting_saas_utils_variables'] as $index => $val) {
    // index is just the number of the child
    $key = $form_state['values']['hosting_saas_utils_variable_key_' . $index];
    $name = $form_state['values']['hosting_saas_utils_variable_name_' . $index];

    if (!empty($key) && !empty($name)) {
      $variables[$index] = array('key' => $key, 'name' => $name);
    }
  }

  variable_set('hosting_saas_utils_variables', $variables);
}

/**
 * Helper function to add a variable set fieldset
 */
function _hosting_saas_utils_append_variable_fieldset(&$form, $index, $variable_key = '', $variable_name = '') {
  $form['hosting_saas_utils_variables_fieldset'][$index] = array(
    '#type' => 'fieldset',
    '#title' => t('Variable set'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // TODO: If key is supplied, value is mandatory.
  $form['hosting_saas_utils_variables_fieldset'][$index]['hosting_saas_utils_variable_key_' . $index] = array(
    '#type' => 'textfield',
    '#title' => t('Variable POST key'),
    '#description' => t("This is the arbitrary name of the POST argument you will give the variable."),
    '#default_value' => $variable_key,
    '#required' => FALSE,
  );

  $form['hosting_saas_utils_variables_fieldset'][$index]['hosting_saas_utils_variable_name_' . $index] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal variable name'),
    '#description' => t("This is the name of the variable that will be set in the new site using variable_set()."),
    '#default_value' => $variable_name,
    '#required' => FALSE,
  );
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function hosting_saas_utils_ajax_callback($form, $form_state) {
  return $form['hosting_saas_utils_variables_fieldset'];
}

/**
 * Submit handler for the "add-more-variable" button.
 *
 * Add empty variable and causes a rebuild.
 */
function hosting_saas_utils_add_variable($form, &$form_state) {
  $form_state['hosting_saas_utils_variables'][] = array('name' => '', 'key' => '');
  $form_state['rebuild'] = TRUE;
}
