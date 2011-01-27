<?php 
// $Id$

/**
 * @file
 * 
 * Define drush commands
 */

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function modulebrowser_drush_command() {
  $items = array();
  
  $items['mb-extract-projects'] = array(
    'callback' => 'modulebrowser_extract_projects',
    'description' => 'Module browser: store database items in files for projects and releases.',
    'aliases' => array('mb-ep'),
  );
  
  return $items;
}

/**
 * Drush command callback
 * 
 * Function used to extract data from database and put it in files.
 * This makes that you have a list directly after installation
 * and also because loading the projects and releases has a drush dependencie at the moment.
 * First load projects, then load releases into file.
 */
function modulebrowser_extract_projects() {
  include_once(drupal_get_path('module', 'modulebrowser').'/modulebrowser.inc');
  
  // Store all projects in one file
  modulebrowser_store_projects();
  
  // Store all releases in one file
  modulebrowser_store_releases();
}
