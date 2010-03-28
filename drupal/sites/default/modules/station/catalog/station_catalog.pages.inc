<?php
// $Id: station_catalog.pages.inc,v 1.2 2009/11/28 22:06:44 drewish Exp $

/**
 * @file
 *
 * This is the old catalog search code that I've replaced with views. I might
 * want to revert to it so I'm keeping a copy of the D6 updated version.
 */

/* // Menu info

  $items['station/catalog/search'] = array(
    'title' => 'Catalog',
    'page callback' => 'station_catalog_search_page',
    'page arguments' => array('', ''),
    'access arguments' => array('view catalog'),
    'type' => MENU_NORMAL_ITEM,
  );
  $items['station/catalog/search/%/%'] = array(
    'title' => 'Catalog',
    'page callback' => 'station_catalog_search_page',
    'page arguments' => array(3, 4),
    'access arguments' => array('view catalog'),
    'type' => MENU_NORMAL_ITEM,
  );
 */

function station_catalog_search_page($field = '', $value ='') {
  $perpage = 30;

  $header = array(
    'number' => array('data' => t('Number'), 'field' => 'sc.number'),
    'artist' => array('data' => t('Artist'), 'field' => 'sc.artist'),
    'album' => array('data' => t('Album'), 'field' => 'sc.album'),
    'year' => array('data' => t('Year'), 'field' => 'sc.year'),
    'label' => array('data' => t('Label'), 'field' => 'sc.label'),
  );

  // Only search when there's a value and the field is valid.
  if (!empty($value) && in_array($field, array('number', 'artist', 'album', 'year', 'label'), TRUE)) {
    $header[$field]['sort'] = 'asc';
    $result = pager_query(db_rewrite_sql("SELECT n.nid, sc.* FROM {node} n INNER JOIN {station_catalog} sc ON n.nid = sc.nid WHERE LOWER(sc.$field) LIKE LOWER('%%%s%%')") . tablesort_sql($header), $perpage, 0, NULL, $value);
  }
  else {
    $header['number']['sort'] = 'desc';
    $result = pager_query(db_rewrite_sql("SELECT n.nid, sc.* FROM {node} n INNER JOIN {station_catalog} sc ON n.nid = sc.nid") . tablesort_sql($header), $perpage, 0);
  }

  $rows = array();
  while ($item = db_fetch_object($result)) {
    $link = 'node/'. $item->nid;
    $rows[] = array(
      array('data' => l($item->number, $link)),
      array('data' => l($item->artist, $link)),
      array('data' => l($item->album, $link)),
      array('data' => $item->year ? $item->year : ''),
      array('data' => $item->label)
    );
  }

  if (!$rows) {
    $rows[] = array(array('colspan' => 4, 'data' => t('No matches were found.')));
  }

  $output = '<div id="station-catalog-search">';
  $output .= drupal_get_form('station_catalog_search_form', $field, $value);
  $output .= '<div id="station-catalog-search-results">';
  $output .= theme('pager', array(), $perpage);
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', array(), $perpage);
  $output .= '</div></div>';
  return $output;
}

function station_catalog_search_form($form_state, $field = '', $value ='') {
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['search']['field'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#options' => array(
      'artist' => t('Artist'),
      'album' => t('Album'),
      'label' => t('Label'),
      'number' => t('Number'),
      'year' => t('Year'),
    ),
    '#default_value' => $field,
  );
  $form['search']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#default_value' => $value,
    '#size' => 25,
  );
  $form['search']['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  if (!empty($value)) {
    $form['search']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }
  return $form;
}

function station_catalog_search_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case t('Reset'):
      $form_state['redirect'] = 'station/catalog/search/';

    case t('Filter'):
      // Make sure it's an allowed search field.
      if (in_array($form_state['values']['field'], array('number', 'artist', 'album', 'year', 'label'))) {
        $form_state['redirect'] = 'station/catalog/search/'. $form_state['values']['field'] .'/'. $form_state['values']['value'];
      }
  }
}