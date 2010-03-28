<?php
// $Id: template.php,v 1.17.2.1 2009/02/13 06:47:44 johnalbin Exp $

/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to fusefm_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: fusefm_breadcrumb()
 *
 *   where fusefm is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and template suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440
 *   and http://drupal.org/node/190815#template-suggestions
 */


/*
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */
/* -- Delete this line if you want to use and modify this code
// Example: optionally add a fixed width CSS file.
if (theme_get_setting('fusefm_fixed')) {
  drupal_add_css(path_to_theme() . '/layout-fixed.css', 'theme', 'all');
}
// */


/**
 * Implementation of HOOK_theme().
 */
function fusefm_theme(&$existing, $type, $theme, $path) {
  $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/*
function fusefm_preprocess(&$vars, $hook) {
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
/* -- Delete this line if you want to use this function
function fusefm_preprocess_page(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function fusefm_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function fusefm_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function fusefm_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the user profile templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("user_profile" in this case.)
 */
function fusefm_preprocess_user_profile(&$vars, $hook) {
  $object = $vars['account'];
  if ($object->uid && $object->name) {
    if ($object->profile_givenName." ".$object->profile_sn) {
      if (drupal_strlen($object->profile_givenName." ".$object->profile_sn) > 20) {
        $name = drupal_substr($object->profile_givenName." ".$object->profile_sn, 0, 15) .'...';
      }
      else {
	$name = $object->profile_givenName." ".$object->profile_sn;
      }
    }
  }
  drupal_set_title(check_plain($name));
}

/**
 * Override or insert variables into the user profile templates.
 *   
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("user_profile" in this case.)
 */
function fusefm_preprocess_page(&$vars, $hook) {
  $template_files = $vars['template_files'];
  if($template_files[0] == 'page-blog') {
  $template_file = explode('-', $template_files[1]);
  $uid = $template_file[2];
  $object = user_load(array('uid' => $uid));
  if ($object->uid && $object->name) {
    if ($object->profile_givenName." ".$object->profile_sn) {
      if (drupal_strlen($object->profile_givenName." ".$object->profile_sn) > 20) {
        $name = drupal_substr($object->profile_givenName." ".$object->profile_sn, 0, 15) .'...';
      }
      else {
        $name = $object->profile_givenName." ".$object->profile_sn;
      }
    }
 
    $output = check_plain($name) . "'s Blog";
  }
  $vars['title'] = $output;
  $vars['head_title'] = $output;
  }
}

/**
 * Format username as full name from profile
 *
 * @param $object
 *   The user object to format.
 * @return $hook
 *   A string containing a link to the user's page if the passed object suggests that this is a site user.
 */
function phptemplate_username($object) {
 
  if ($object->uid && $object->name) {
    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($object->name) > 20) {
      $name = drupal_substr($object->name, 0, 15) .'...';
    }
    else {
      $name = $object->name;
    }
    $profile = user_load(array('uid' => $object->uid));
    if ($profile->profile_givenName." ".$profile->profile_sn) {
      if (drupal_strlen($profile->profile_givenName." ".$profile->profile_sn) > 20) {
        $name = drupal_substr($profile->profile_givenName." ".$profile->profile_sn, 0, 15) .'...';
      }
      else {
        $name = $profile->profile_givenName." ".$profile->profile_sn;
      }
    }
 
    if (user_access('access user profiles')) {
      $output = l($name, 'user/'. $object->uid, array('title' => t('View user profile.')));
    }
    else {
      $output = check_plain($name);
    }
  }
  else if ($object->name) {
    // Sometimes modules display content composed by people who are
    // not registered members of the site (e.g. mailing list or news
    // aggregator modules). This clause enables modules to display
    // the true author of the content.
    if ($object->homepage) {
      $output = l($object->name, $object->homepage);
    }
    else {
      $output = check_plain($object->name);
    }
 
    $output .= ' ('. t('not verified') .')';
  }
  else {
    $output = variable_get('anonymous', t('Anonymous'));
  }
 
  return $output;
}
