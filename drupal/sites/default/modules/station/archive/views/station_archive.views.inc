<?php

// $Id: station_archive.views.inc,v 1.1 2009/11/28 20:55:19 drewish Exp $

/**
 * Implementation of hook_views_data().
 */
function station_archive_views_data() {
  $data['station_archive_item']['table']['group']  = t('Station Archive');
  $data['station_archive_item']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'audio_nid'
    ),
  );

  $data['station_archive_item']['aired'] = array(
    'title' => t('Air date'),
    'help' => t('Date the audio was aired.'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'views_handler_field_date',
    ),
  );
  $data['station_archive_item']['imported'] = array(
    'title' => t('Import date'),
    'help' => t('Date the audio was imported into the station archive.'),
    'field' => array(
      'sortable' => TRUE,
      'handler' => 'views_handler_field_date',
      'option' => 'string',
    ),
  );
  $data['station_archive_item']['permanent'] = array(
    'title' => t('Is in permanent archive'),
    'help' => t('Is this audio node part of the permanent archive?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Is permanent'),
      'type' => 'yes-no',
    ),
  );
  $data['station_archive_item']['program_nid'] = array(
    'title' => t('Program Nid'),
    'help' => t("The program node id that the audio is a recording of."),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'title' => t('Program'),
      'help' => t('Add a relationship to gain access to data about the program the audio is a recording of. Note: this relationship only works if the archive and schedule are on the same site.'),
      'base' => 'node',
      'base field' => 'nid',
      'relationship table' => 'station_archive_item',
      'relationship field' => 'program_nid',
      'handler' => 'views_handler_relationship',
      'label' => t("Archived audio's program"),
    ),
  );


  $data['station_archive_program']['table']['group']  = t('Station Archive');
  $data['station_archive_program']['table']['join'] = array(
    'station_archive_item' => array(
       'table' => 'station_archive_program',
       'left_field' => 'program_nid',
       'field' => 'program_nid',
    ),
  );
  $data['station_archive_program']['title'] = array(
    'title' => t('Program title'),
    'help' => t('The program that this audio is a recording of.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'sortable' => TRUE,
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
//function station_archive_views_handlers() {
//  return array(
//    'info' => array(
//      'path' => drupal_get_path('module', 'station_archive') . '/views',
//    ),
//    'handlers' => array(
//      // Field handlers
//      'station_schedule_handler_field_streams' => array(
//        'parent' => 'views_handler_field',
//      ),
//    ),
//  );
//}

// TODO port this to a views2 field handler:
function station_archive_handler_audio_program_title_field($fieldinfo, $fielddata, $value, $data) {
  $schedule = station_default_schedule();
  if ($fielddata['options'] == 'link' && isset($schedule['base_url']) && !empty($data->station_archive_program_program_nid)) {
    return l($value, $schedule['base_url'] .'/node/'. $data->station_archive_program_program_nid);
  }
  return check_plain($value);
}
