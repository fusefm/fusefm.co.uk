<?php
// $Id: blocks404.active.inc,v 1.5 2009/06/29 19:14:09 johnalbin Exp $

/**
 * Helper function that performs the actual inclusion of the left and right regions.
 */
function _blocks404_preprocess_page(&$vars) {
  // Render the left and right regions.
  $vars['left']  = theme('blocks', 'left')  . $vars['left'];
  $vars['right'] = theme('blocks', 'right') . $vars['right'];

  // Add JS/CSS added by blocks.
  $vars['scripts'] = drupal_get_js();
  $vars['styles'] = drupal_get_css();

  // Some themes (like Acquia Marina) are checking the show_blocks variable to
  // hide regions that aren't named left or right.
  $vars['show_blocks'] = TRUE;

  blocks404_reset_body_classes($vars);
}

/**
 * Resets the body classes if called from within a preprocess_page function.
 */
function blocks404_reset_body_classes(&$vars) {
  // Determine the new layout variable.
  $layout = 'none';
  if (!empty($vars['left'])) {
    $layout = 'left';
  }
  if (!empty($vars['right'])) {
    $layout = ($layout == 'left') ? 'both' : 'right';
  }

  // If the new layout is different than the old layout, reset the body classes.
  if ($layout != $vars['layout']) {
    // Some themes (like Zen) store body classes in an array.
    if (!empty($vars['body_classes_array'])) {
      $classes = $vars['body_classes_array'];
    }
    elseif (!empty($vars['classes_array'])) {
      $classes = $vars['classes_array'];
    }
    else {
      // Otherwise, we just act on core's $body_classes.
      $classes = explode(' ', $vars['body_classes']);
    }

    // Undo the old body classes. $pos will never be 0 because the first body
    // class is always front/not-front; see template_preprocess_page().
    if ($vars['layout'] == 'both' && $pos = array_search('two-sidebars', $classes)) {
      unset($classes[$pos]);
    }
    else if ($vars['layout'] == 'none' && $pos = array_search('no-sidebars', $classes)) {
      unset($classes[$pos]);
    }
    else {
      if ($pos = array_search('one-sidebar', $classes)) {
        unset($classes[$pos]);
      }
      if ($pos = array_search('sidebar-' . $vars['layout'], $classes)) {
        unset($classes[$pos]);
      }
    }

    // Save the new layout variable.
    $vars['layout'] = $layout;

    // Add information about the number of sidebars.
    if ($layout == 'both') {
      $classes[] = 'two-sidebars';
    }
    elseif ($layout == 'none') {
      $classes[] = 'no-sidebars';
    }
    else {
      $classes[] = 'one-sidebar';
      $classes[] = 'sidebar-' . $layout;
    }

    // Save the new body classes variables.
    if (!empty($vars['body_classes_array'])) {
      $vars['body_classes_array'] = $classes;
    }
    elseif (!empty($vars['classes_array'])) {
      $vars['classes_array'] = $classes;
    }
    $vars['body_classes'] = implode(' ', $classes);
  }
}

/**
 * Implements hook_form_alter().
 *
 * We need this to be able to submit any forms from the error pages, otherwise
 * the form POSTs to the error page and the form is not processed.
 */
function _blocks404_form_alter(&$form, $form_state, $form_id) {
  if ($_GET['q'] == BLOCKS404_PAGE) {
    // Form actions that POST to the 404 page won't work properly.
    if ($form['#action'] == url(BLOCKS404_PAGE) || $form['#action'] == url(BLOCKS404_ORIGINAL_QUERY) || $form_id == 'user_login_block' || $form['#action'] == url(BLOCKS404_PAGE, array('query' => 'destination=' . BLOCKS404_ORIGINAL_QUERY))) {
      $form['#action'] = url('<front>');
    }
    // Form actions that redirect back to the 404 page aren't good either.
    elseif (strpos($form['#action'], 'destination=' . urlencode(BLOCKS404_ORIGINAL_QUERY)) !== FALSE) {
      // Deconstruct the URL.
      list(, $path) = explode($GLOBALS['base_path'], $form['#action']);
      list($path, $query) = explode('?', $path);
      $query = explode('&', $query);
      if (($pos = array_search('destination=' . urlencode(BLOCKS404_ORIGINAL_QUERY), $query)) !== FALSE) {
        // Remove the broken redirection.
        unset($query[$pos]);
        // Reconstruct the URL.
        $form['#action'] = url($path, array('query' => $query));
      }
    }
  }
}
