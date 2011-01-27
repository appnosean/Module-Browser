<?php 
// $Id$

/**
 * @file
 * 
 * Default theming implementation for displaying list of projects.
 * 
 * 
 * 
 * @see template_preprocess_modulebrowser_list()
 */
?>
<?php if ($projects_list) :?>
  <div class="module-browser">
  <!-- 
    <div class="module-browser-header">
      <?php //print $header; ?>
    </div>  -->
    <?php print $list; ?>
    <?php //print $pager; ?>
  </div>
<?php else :?><!-- ENDIF / START ELSE -->
  <div class="module-browser">
    <h2><?php print t('No results found.')?></h2>
  </div>
<?php endif; ?>
<div class="sidebar-right">
  <?php print $filter; ?>
</div>
