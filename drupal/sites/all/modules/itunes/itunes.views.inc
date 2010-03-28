<?php
// $Id: itunes.views.inc,v 1.2 2009/07/31 22:45:06 drewish Exp $

/**
 * @file
 * Views includes.
 */

/**
 * Implementation of hook_views_plugins().
 */
function itunes_views_plugins() {
  return array(
    'module' => 'itunes',
    'style' => array(
      'itunes_rss' => array(
        'title' => t('iTunes RSS Feed'),
        'help' => t('Generates an RSS feed from a view with extra iTunes specific attributes.'),
        'handler' => 'itunes_plugin_style_rss',
        'theme' => 'views_view_rss',
        'parent' => 'rss',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
      ),
    ),
  );
}