<?php
// $Id: station_catalog.views_default.inc,v 1.4 2009/04/19 21:34:12 drewish Exp $


/**
 * Implementation of hook_default_view_views().
 */
function station_catalog_views_default_views() {
  $view = new view;
  $view->name = 'station_catalog';
  $view->description = 'Station Catalog search view';
  $view->tag = 'station';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'number' => array(
      'label' => 'Number',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'number',
      'table' => 'station_catalog',
      'field' => 'number',
      'relationship' => 'none',
    ),
    'artist' => array(
      'label' => 'Artist',
      'exclude' => 0,
      'id' => 'artist',
      'table' => 'station_catalog',
      'field' => 'artist',
      'relationship' => 'none',
    ),
    'year' => array(
      'label' => 'Year',
      'exclude' => 0,
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'id' => 'year',
      'table' => 'station_catalog',
      'field' => 'year',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'label' => array(
      'label' => 'Label',
      'exclude' => 0,
      'id' => 'label',
      'table' => 'station_catalog',
      'field' => 'label',
      'relationship' => 'none',
    ),
    'edit_node' => array(
      'label' => '',
      'text' => 'Edit',
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'station_album' => 'station_album',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'status_extra' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status_extra',
      'table' => 'node',
      'field' => 'status_extra',
      'relationship' => 'none',
    ),
    'artist' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'artist_op',
        'identifier' => 'artist',
        'label' => 'Artist',
        'optional' => 1,
        'remember' => 1,
      ),
      'case' => 0,
      'id' => 'artist',
      'table' => 'station_catalog',
      'field' => 'artist',
      'relationship' => 'none',
    ),
    'label' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'label_op',
        'identifier' => 'label',
        'label' => 'Label',
        'optional' => 1,
        'remember' => 1,
      ),
      'case' => 0,
      'id' => 'label',
      'table' => 'station_catalog',
      'field' => 'label',
      'relationship' => 'none',
    ),
    'year' => array(
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'year_op',
        'identifier' => 'year',
        'label' => 'Year',
        'optional' => 1,
        'remember' => 1,
      ),
      'id' => 'year',
      'table' => 'station_catalog',
      'field' => 'year',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view station album content',
  ));
  $handler->override_option('title', 'Catalog');
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'number' => 'number',
      'artist' => 'artist',
      'label' => 'label',
      'asin' => 'asin',
      'year' => 'year',
      'mb_release_id' => 'mb_release_id',
    ),
    'info' => array(
      'number' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'artist' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'label' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'asin' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'year' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'mb_release_id' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => 'number',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'station/catalog/search');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'Catalog',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));
  $views[$view->name] = $view;

  return $views;
}
