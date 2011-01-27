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
function modulebrowser_scraper_drush_command() {
  $items = array();
  
  $items['mb-load-projects'] = array(
    'callback' => 'modulebrowser_scraper_load_projects',
    'description' => 'Module browser: loads all modules / themes from XML (project module) and gets extra information using scraping.',
    'aliases' => array('mb-lp'),
  );
  
  return $items;
}

/**
 * Drush command callback
 */
function modulebrowser_scraper_load_projects() {
  include_once(drupal_get_path('module', 'modulebrowser_scraper').'/modulebrowser_scraper.module');
  
  // First process all projects
  modulebrowser_scraper_update_all_projects();
  
  // Next process all releases
  modulebrowser_scraper_all_releases();
  
  // Finally get extra data using scraping.
  modulebrowser_scraper_scrape_all();
}
