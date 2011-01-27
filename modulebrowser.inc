<?php 
// $Id$

/**
 * @file
 *
 */

/**
 * Implements hook_form().
 * From to install selected module
 * TODO: Form to add/remove module from install list.
 * 
 * @param integer $pid    Project ID
 */
function modulebrowser_install_form($form, &$form_state, $pid) {
  $status = 0;
  
  $form['install'] = array(
    '#type' => 'checkbox',
    '#prefix' => '<div class="install-item-prefix">'.t('Add to list').'</div>',
    '#default_value' => $status,
    '#attributes' => array('onchange' => 'this.form.submit();'),
  );
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Install',
    '#submit' => array('modulebrowser_install_form_submit'),
    '#attributes' => array('style' => 'display: none;')
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 */
function modulebrowser_install_form_submit($form, &$form_state) {
  // Install selected module
  if ($form_state['submitted']) {
    drupal_goto('admin/modulebrowser/install/'.$form_state['values']['pid']);
  }
}

/**
 * Form to get all available version filters
 */
function modulebrowser_version_filter_form($form, &$form_state) {
  $versions = modulebrowser_get_versions();
  
  $form['version'] = array(
    '#type' => 'fieldset',
    '#title' => t('Version'),
    '#collapsible' => FALSE,
  );
  
  foreach ($versions as $version) {
    $form_version = str_replace('.', '_', $version);
    $form['version'][$form_version] = array(
      '#type' => 'checkbox',
      '#title' => $version,
      '#default_value' => isset($_SESSION['modulebrowser_version'][$version]) ? $_SESSION['modulebrowser_version'][$version] : 0,  
      '#attributes' => array('onchange' => 'this.form.submit()'),
    );
  }
  
  $form['version']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
    '#attributes' => array('style' => 'display: none;')
  );
  
  return $form;
}

/**
 * Submit handler for setting filter 
 */
function modulebrowser_version_filter_form_submit($form, &$form_state) {
  if ($form_state['submitted']) {
    $protected = array('submit', 'form_build_id', 'form_token', 'form_id', 'op');
    foreach ($form_state['values'] as $key => $value) {
      if (!in_array($key, $protected)) {
        $session_key = str_replace('_', '.', $key);
        $_SESSION['modulebrowser_version'][$session_key] = $value;
      }
    }
  }
}

/**
 * Form to get all available categories. 
 */
function modulebrowser_category_filter_form($form, &$form_state) {
  $categories = modulebrowser_get_categories();
  $form['category'] = array(
    '#type' => 'fieldset',
    '#title' => t('Category'),
    '#collapsible' => FALSE,
  );
  
  foreach ($categories as $category) {
    $form_category = str_replace(' ', '_', $category);
    $form['category'][$form_category] = array(
      '#type' => 'checkbox',
      '#title' => $category,
      '#default_value' => isset($_SESSION['modulebrowser_category'][$category]) ? $_SESSION['modulebrowser_category'][$category] : 0,
      '#attributes' => array('onchange' => 'this.form.submit()'),
    );
  }
  
  $form['category']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
    '#attributes' => array('style' => 'display: none;')
  );
  
  return $form;
}

/**
 * Submit handler for setting filter
 */
function modulebrowser_category_filter_form_submit($form, &$form_state) {
if ($form_state['submitted']) {
    $protected = array('submit', 'form_build_id', 'form_token', 'form_id', 'op');
    foreach ($form_state['values'] as $key => $value) {
      if (!in_array($key, $protected)) {
        $session_key = str_replace('_', ' ', $key);
        $_SESSION['modulebrowser_category'][$session_key] = $value;
      }
    }
  }
}

/**
 * Helper function to get all projects.
 *
 * @return stdClass
 *    Object with all projects (project ID and short_name)
 */
function modulebrowser_get_projects() {
  $projects = db_query("SELECT pid, short_name FROM {modulebrowser_projects}")->fetchAll();
  
  return $projects;
}

/**
 * Helper function to get install list.
 * Install list is all projects that are selected to install. 
 */
function modulebrowser_get_install_list() {
  
}

/**
 * Helper function to get available sortings.
 * TODO: implement function
 */
function modulebrowser_sort_list() {
  
}

/**
 * Helper function to get all available versions.
 * 
 *  @return array
 *      Array containing all available version
 */
function modulebrowser_get_versions() {
  $versions = array();
  $protected = array('HEAD');
  $query = db_query("SELECT DISTINCT(version) FROM {modulebrowser_releases}");
  
  foreach ($query as $project_version) {
    $version = explode('-', $project_version->version);
    if (!in_array($version[0] ,$protected)) {
      $versions[$version[0]] = $version[0];
    }
  }
  
  ksort($versions);
  return $versions;
}

/**
 * Helper function to get all available categories.
 * 
 * @return array
 *    Array containing all available categories
 */
function modulebrowser_get_categories() {
  $categories = array();
  $protected = array('Drupal project', 'Modules', 'Themes');
  $query = db_query("SELECT terms FROM {modulebrowser_projects}");
  
  foreach ($query as $term) {
    $category = $term->terms;
    if(!empty($category)) {
      $category_array = explode(', ', $category);
      if((is_array($category_array)) && (!empty($category_array)) && ($category)) {
        foreach ($category_array as $single_item) {
          if(!in_array($single_item, $protected)) {
            $categories[$single_item] = $single_item;
          }
        }
      }
      else {
        $categories[$category] = $category;
      }
    }
  }
  
  ksort($categories);
  return $categories;
}

/********************************************************************************
 * Functions to extract database entries to file
 ********************************************************************************/

/**
 * Function to store all projects in file
 */
function modulebrowser_store_projects() {
  $path = drupal_get_path('module', 'modulebrowser');
  include_once($path . '/modulebrowser.csv.inc');
  
  $projects = array();
  $results = db_query("SELECT * FROM {modulebrowser_projects}");
  
  foreach ($results as $project) {
    $row['pid'] = $project->pid;
    $row['title'] = $project->title;
    $row['short_name'] = $project->short_name;
    $row['description'] = $project->description;
    $row['link'] = $project->link;
    $row['screenshot_url'] = $project->screenshot_url;
    $row['status'] = $project->status;
    $row['type'] = $project->type;
    $row['terms'] = $project->terms;
    $row['changed'] = $project->changed;

    $projects[] = $row;
  }
  
  $header = array_keys($projects[0]);
  
  // Store file
  modulebrowser_export_to_csv($path, 'projects.csv', $header, $projects, array());
  
  return TRUE;
}

/**
 * Function to store all releases in file
 */
function modulebrowser_store_releases() {
  $path = drupal_get_path('module', 'modulebrowser');
  include_once($path . '/modulebrowser.csv.inc');
  
  $releases = array();
  $results = db_query("SELECT * FROM {modulebrowser_releases}");
  
  foreach ($results as $release) {
    $row['rid'] = $release->rid;
    $row['pid'] = $release->pid;
    $row['version'] = $release->version;
    $row['tag'] = $release->tag;
    $row['version_major'] = $release->version_major;
    $row['version_extra'] = $release->version_extra;
    $row['version_patch'] = $release->version_patch;
    $row['date'] = $release->date;
    $row['download_link'] = $release->download_link;
    $row['hash'] = $release->hash;
    $row['filesize'] = $release->filesize; 
    
    $releases[] = $row;
  }
  
  $header = array_keys($releases[0]);
  
  // Store file
  modulebrowser_export_to_csv($path, 'releases.csv', $header, $releases, array());
  
  return TRUE;
}
