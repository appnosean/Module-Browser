<?php 
// $Id$

/**
 * @file
 * 
 *  Module to handle XMLRPC interface for module browser.
 *  XMLRPC is used to synchronize the projects data, for all available projects.
 */

/**
 * Function to get all projects
 * @return unknown_type
 */
function modulebrowser_xmlrpc_get_projects() {
  // Default values;
  $method = 'project.projects';
  $args = array(
    'fields' => array(
      'title' => 0,
    ),
  );
  
  // Make xmlrpc request
  $projects = modulebrowser_xmlrpc_request($method, $args);
}

/**
 * Function to get all releases
 * @return unknown_type
 */
function modulebrowser_xmlrpc_get_releases() {
  // Default values;
  $method = 'project.releases';
  $args = array(
    'fields' => array(
      'title' => 0,
    ),
  );
  
  // Make xmlrpc request
  $updates = modulebrowser_xmlrpc_request($method, $args);
}

/**
 * Function to get all updates (needs timestamp of last updated)
 * @return unknown_type
 */
function modulebrowser_xmlrpc_get_update() {
  // Default values;
  $method = 'project.update';
  $args = array(
    'fields' => array(
      'title' => 0,
      'type' => 0,
    ),
  );
  
  // Make xmlrpc request
  $updates = modulebrowser_xmlrpc_request($method, $args);
}

/**
 * Send XMLRPC request and check result.
 * 
 * @param string $method
 *    XMLRPC request to preform on server
 * @param array $args
 *    Arguments to send to server
 * @return array
 *    Result from XMLRPC request
 */
function modulebrowser_xmlrpc_request($method = '', $args = array()) {
  // TODO: make list (repository style) or at least configurable.
  $path = 'http://stekke-laptop.project/xmlrpc.php';
  
  $xmlrpc = xmlrpc($path, $method, $args);
  
  if (!$xmlrpc) {
    $error_code = xmlrpc_errno();
    $error_message = xmlrpc_error_msg();
    //Report error.
    watchdog('module xmlrpc', t('Error while preforming xmlrpc request (%request), error:<br> Code = %code <br> Message = %message'), array('%request' => $method, '%code' => $error_code, '%message' => $error_message), WATCHDOG_ERROR);
    drupal_set_message(t('An error occurd while trying to renew the projects data, please notify administrator.'));
    
    return FALSE;
  }
  
  return $xmlrpc;
}
