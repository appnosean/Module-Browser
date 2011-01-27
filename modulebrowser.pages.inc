<?php 
// $Id$

/**
 * @file
 * 
 */

define('PROJECT_ENABLED', '1');
define('PROJECT_DOWNLOADED', '0');
define('PROJECT_INSTALL', NULL);

/**
 * Page to display the projects.
 * 
 * @param string $type
 *      Type of project to view
 * @return array
 *      Array containing all results that match the query
 */
function modulebrowser_page($type) {
  drupal_add_css(drupal_get_path('module', 'modulebrowser') . '/modulebrowser.css', array('preprocess' => FALSE));
  
  $list = array();
  
  $query = db_select('modulebrowser_projects', 'p');
  $query->leftJoin('system', 's', 'p.short_name = s.name');
  $query 
      ->condition('p.status', 1, '=')
      ->condition('p.type', $type, '=')
      ->fields('p', array('pid', 'title', 'short_name', 'link', 'description'))
      ->addField('s', 'status', 'system_status');
  
  // Check filters
  if (isset($_SESSION['modulebrowser_category'])) {
    foreach($_SESSION['modulebrowser_category'] as $key => $value) {
      if($value) {
        $query->condition('p.terms', $key, 'LIKE');
      }
    }
  }
  if (isset($_SESSION['modulebrowser_version'])) {
    foreach($_SESSION['modulebrowser_version'] as $key => $value) {
      if($value) {
        $query->condition('r.version', $key.'%', 'LIKE');
      }
    }
  }
  // Check sort
  if (isset($_SESSION['modulebrowser_sort'])) {
    foreach($_SESSION['modulebrowser_sort'] as $key => $value) {
      if ($value != 0) {
        $query->orderBy('p.'.$key, 'ASC');
      }
    }
  } 
  else {
    $query->orderBy('p.title', 'ASC');
  }
  // TODO: debug, because pager not working.
  $query->extend('PagerDefault')->limit(10);
  $projects = $query
      ->execute();
  
  foreach ($projects as $project) {
    $list[] = $project;
  }
  
  return theme('modulebrowser_list', array('projects_list' => $list));
}

/**
 * Page to install a module
 * 
 * @param string $op
 *    Operation to preform.
 * @param integer $pid
 *    Project ID
 */
function modulebrowser_installation_page($op, $pid) {
  switch ($op) {
    case 'install':
      include_once (drupal_get_path('module', 'update') . '/update.manager.inc' );
      $project_url = db_query("SELECT download_link FROM {modulebrowser_releases} WHERE pid = :pid ORDER BY date DESC", array(':pid' => $pid))->fetchField();
      if(!$project_url) {
        $title = db_query("SELECT title FROM {modulebrowser_projects} WHERE pid = :pid", array(':pid' => $pid))->fetchField();
        drupal_set_message('No release found for project ' . $title, 'error', TRUE);
        drupal_goto('admin/modulebrowser');
      }
      
      // TODO: Check if dev version and if only dev version available report to user.
      
      // Fill in values.
      $form_state = array();
      $form_state['values']['project_url'] = $project_url;
      $form_state['values']['op'] = t('Install');
      // @todo: determine type of project (module, theme, ...)
      $context = 'module';
      
      drupal_form_submit('update_manager_install_form', $form_state, $context);
      drupal_goto('admin/modulebrowser');
      break;
  }
  
}

/**
 * Theme the list of projects.
 * 
 * @param $variables
 *    An associative array containing:
 *     - projects_list : array of all projects
 *     
 * @return
 *    A string containing the listing output.
 */
function template_preprocess_modulebrowser_list(&$variables) {
  include_once(drupal_get_path('module', 'modulebrowser') . '/modulebrowser.inc');
  
  $variables['list'] = '';
  
  foreach ($variables['projects_list'] as $project) {
    $variables['list'] .= theme('modulebrowser_project', array('project' => $project));
  }
  
  $variables['pager'] = theme('pager', array('tags' => NULL));
  
  // Add the available version to filter on
  $version_form = drupal_get_form('modulebrowser_version_filter_form');
  $content = drupal_render($version_form);
  // Add the available category to filter on
  $category_form = drupal_get_form('modulebrowser_category_filter_form');
  $content .= drupal_render($category_form); 
  // Add the sorting of the list
  $content .= modulebrowser_sort_list();
  $variables['filter'] = $content;
  
  //$variables['install_list'] = modulebrowser_get_install_list();
}

/**
 * Theme one projects.
 * 
 * @param $variables
 *    An associative array containing:
 *     - project : project object
 *     
 * @return
 *    A string containing the listing output.
 */
function template_preprocess_modulebrowser_project(&$variables) {
  include_once(drupal_get_path('module', 'modulebrowser') . '/modulebrowser.inc');
  
  $project = $variables['project'];
  
  $variables['title'] = l(check_plain($project->title), check_url($project->link));
  $variables['description'] = $project->description;
  switch ($project->system_status) {
    case PROJECT_ENABLED:
      $variables['status'] = '<div class="install-disabled">Already installed</div>'; 
      $variables['install'] = '';
      break;
    case PROJECT_DOWNLOADED:
      $variables['status'] = '<div class="install-disabled">Already downloaded</div>';
      $variables['install'] = '';
      break;
    case PROJECT_INSTALL:
      $variables['status'] = '<div class="install-enabled">Install</div>';
      $install_form = drupal_get_form('modulebrowser_' . $project->short_name . '_install_form', $project->pid);
      $variables['install'] = drupal_render($install_form);
      break;
  }
}
