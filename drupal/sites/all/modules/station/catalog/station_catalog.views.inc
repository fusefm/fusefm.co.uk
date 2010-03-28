<?php
// $Id: station_catalog.views.inc,v 1.3 2009/08/29 08:01:42 drewish Exp $


/**
 * Implementation of hook_views_data().
 */
function station_catalog_views_data() {
  // Basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['station_catalog']['table']['group']  = t('Station Catalog');

  // For other base tables, explain how we join
  $data['station_catalog']['table']['join'] = array(
    'node' => array(
      'handler' => 'views_join', // this is actually optional
      'arguments' => array('station_catalog', 'node', 'nid', 'nid'),
    ),
  );

  // ----------------------------------------------------------------
  // Fields
  $data['station_catalog']['number'] = array(
    'title' => t('Catalog Number'),
    'help' => t("The album's catalog number."),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_numeric',
       'label' => t('Number'),
      ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_catalog']['artist'] = array(
    'title' => t('Artist'),
    'help' => t("Album artist name."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Artist'),
      ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_catalog']['album'] = array(
    'title' => t('Album title'),
    'help' => t("Album title."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Album'),
      ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_catalog']['year'] = array(
    'title' => t('Year'),
    'help' => t("Year the album was released."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_numeric',
       'label' => t('Year'),
      ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_catalog']['label'] = array(
    'title' => t('Label'),
    'help' => t("Record label that released the album."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Label'),
      ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_catalog']['mb_release_id'] = array(
    'title' => t('Music Brainz ID'),
    'help' => t("MusicBrainz ID for this release."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Music Brainz ID'),
      ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_catalog']['asin'] = array(
    'title' => t('ASIN'),
    'help' => t("Amazon product ID for this album."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Active'),
      ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}
