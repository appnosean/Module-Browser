<?php 
// $Id$

/**
 * @file
 * 
 */

/**
 * Implements hook_form()
 * 
 * Settings page for module browser.
 */
function modulebrowser_settings_form($form, $form_state) {
  $form['modulebrowser'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['modulebrowser']['modulebrowser_debugging'] = array(
    '#type' => 'select',
    '#title' => t('Debug mode'),
    '#options' => array('OFF', 'ON'),
  );
  
  $form['modulebrowser']['modulebrowser_refresh'] = array(
    '#type' => 'select',
    '#title' => t('Select refresh rate'),
    '#options' => array(
      '600' => t('10 minutes'),
      '1200' => t('20 minutes'),
      '1800' => t('30 minutes'),
      '3600' => t('1 hour'),
      '7200' => t('2 hours'),
      '10800' => t('3 hours'),
      '14400' => t('4 hours'),
      '28800' => t('8 hours'),
      '43200' => t('12 hours'),
      '86400' => t('1 day'),
      '172800' => t('2 days'),
      '259200' => t('3 days'),
      '345600' => t('4 days'),
      '432000' => t('5 days'),
      '518400' => t('6 days'),
      '604800' => t('1 week'),
    ),
  );
  
  return system_settings_form($form);
}

