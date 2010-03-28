<?php
// $Id: station_schedule.views.inc,v 1.11 2009/09/22 23:47:56 drewish Exp $

/**
 * Implementation of hook_views_handlers().
 */
function station_schedule_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'station_schedule') . '/views',
    ),
    'handlers' => array(
      // Field handlers
      'station_schedule_handler_field_streams' => array(
        'parent' => 'views_handler_field',
      ),
      'station_schedule_handler_field_time' => array(
        'parent' => 'views_handler_field',
      ),
      'station_schedule_handler_field_time_range' => array(
        'parent' => 'views_handler_field',
      ),
      'station_schedule_handler_field_iid_link' => array(
        'parent' => 'views_handler_field',
      ),
      'station_schedule_handler_field_iid_link_edit' => array(
        'parent' => 'station_schedule_handler_field_iid_link',
      ),
      'station_schedule_handler_field_iid_link_remove' => array(
        'parent' => 'station_schedule_handler_field_iid_link',
      ),

      // Sort handlers
      'station_schedule_handler_sort_time' => array(
        'parent' => 'views_handler_sort',
      ),

      // Argument handlers
      'station_schedule_handler_argument_day' => array(
        'parent' => 'views_handler_argument',
      ),

      // Filter handlers
      'station_schedule_handler_filter_time' => array(
        'parent' => 'views_handler_filter_numeric',
      ),
      'station_schedule_handler_filter_schedule_item' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function station_schedule_views_plugins() {
  return array(
    'module' => 'station_schedule', // This just tells our themes are elsewhere.
//    'argument validator' => array(
//      'station_schedule_day' => array(
//        'title' => t('Validate day'),
//        'handler' => 'station_schedule_plugin_argument_validate_day',
//      ),
//    ),
  );
}

/**
 * Implementation of views_query_substitutions().
 */
function station_schedule_views_query_substitutions($view) {
  $minute = station_minute_from_local_ts();
  return array(
    '***STATION_CURRENT_MINUTE***' => $minute,
    '***STATION_CURRENT_DAY_IN_MINUTES***' => $minute - ($minute % MINUTES_IN_DAY),
  );
}

/**
 * Implementation of hook_views_data().
 */
function station_schedule_views_data() {
  // ----------------------------------------------------------------
  // Basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['station_schedule']['table']['group']  = t('Station Schedule');

  // For other base tables, explain how we join
  $data['station_schedule']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // ----------------------------------------------------------------
  // Fields
  $data['station_schedule']['increment'] = array(
    'title' => t('Scheduling increment'),
    'help' => t("The schedule's block size in minutes."),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_numeric',
       'label' => t('Increment'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_schedule']['streams'] = array(
    'title' => t('Webstream'),
    'help' => t("Link to M3U files with stream URLs."),
    'field' => array(
      'handler' => 'station_schedule_handler_field_streams',
    ),
  );
  $data['station_schedule']['unscheduled_message'] = array(
    'title' => t('Unscheduled message'),
    'help' => t("Message displayed when there is no scheduled programming."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Unscheduled message'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );



  // ----------------------------------------------------------------
  // Basic table information.

  $data['station_schedule_item']['table']['group']  = t('Station Schedule Item');
  $data['station_schedule_item']['table']['base'] = array(
    'field' => 'iid',
    'title' => t('Station Schedule Item ID'),
    'help' => t("Items on the station's schedules."),
  );

  // ----------------------------------------------------------------
  // Fields
  $data['station_schedule_item']['iid'] = array(
    'title' => t('Schedule item id'),
    'help' => t("The schedule item's id."),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );
  $data['station_schedule_item']['link_iid'] = array(
    'field' => array(
      'title' => t('View link'),
      'help' => t('Provide a simple link to the schedule item.'),
      'handler' => 'station_schedule_handler_field_iid_link',
    ),
  );
  $data['station_schedule_item']['edit_iid'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the schedule item.'),
      'handler' => 'station_schedule_handler_field_iid_link_edit',
    ),
  );
  $data['station_schedule_item']['remove_iid'] = array(
    'field' => array(
      'title' => t('Remove link'),
      'help' => t('Provide a simple link to remove the schedule item.'),
      'handler' => 'station_schedule_handler_field_iid_link_remove',
    ),
  );
  $data['station_schedule_item']['schedule_nid'] = array(
    'title' => t('Schedule'),
    'help' => t("The schedule this slot belongs to."),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'field' => 'schedule_nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Schedule'),
    ),
  );
  $data['station_schedule_item']['program_nid'] = array(
    'title' => t('Program'),
    'help' => t("The program scheduled in this slot."),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'field' => 'schedule_nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Program'),
    ),
  );
  $data['station_schedule_item']['time'] = array(
    'title' => t('Time range'),
    'help' => t("Schedule instance time range."),
    'field' => array(
      'handler' => 'station_schedule_handler_field_time_range',
      'additional fields' => array('start', 'finish'),
    ),
    'argument' => array(
      'handler' => 'station_schedule_handler_argument_day',
      'label' => t('Day of week'),
    ),
    'sort' => array(
      'handler' => 'station_schedule_handler_sort_time',
    ),
  );
  $data['station_schedule_item']['start'] = array(
    'title' => t('Start'),
    'help' => t("Schedule instance start time."),
    'field' => array(
      'handler' => 'station_schedule_handler_field_time',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'station_schedule_handler_filter_time',
      'allow empty' => FALSE,
      'label' => t('Start'),
    ),
    'sort' => array(
      'handler' => 'station_schedule_handler_sort_time',
    ),
  );
  $data['station_schedule_item']['finish'] = array(
    'title' => t('Finish'),
    'help' => t("Schedule instance completion time."),
    'field' => array(
      'handler' => 'station_schedule_handler_field_time',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'station_schedule_handler_filter_time',
      'allow empty' => FALSE,
      'label' => t('Finish'),
    ),
    'sort' => array(
      'handler' => 'station_schedule_handler_sort_time',
    ),
  );
  $data['station_schedule_item']['may_archive'] = array(
    'title' => t('May be archived'),
    'help' => t("Is this program allowed to be stored in the Station Archive."),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Active'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

/**
 * Use views_data_alter to add relations from {node} to {schedule_items}.
 */
function station_schedule_views_data_alter(&$data) {
  $data['node']['station_schedule_iid'] = array(
    'group' => t('Station Schedule'),
    'title' => t('Scheduled Programs'),
    'help' => t('All programs on this schedule.'),
    'real field' => 'nid',
//    'field' => array(
//      'handler' => 'views_handler_field_upload_fid',
//    ),
    'filter' => array(
      'handler' => 'station_schedule_handler_filter_schedule_item',
      'title' => t('Has scheduled programs'),
      'type' => 'yes-no',
      'label' => t('Has scheduled programs'),
      'help' => t('Only display schedules with programs. This can cause duplicates if there are multiple programs scheduled.'),
    ),
    'relationship' => array(
      'title' => t('Scheduled Programs'),
      'help' => t('Add a relationship to gain access to data about the programs on a schedule. Note that this relationship will cause duplicate schedule nodes if there are multiple programs on the schedule.'),
      'relationship table' => 'node',
      'relationship field' => 'nid',
      'base' => 'station_schedule_item',
      'base field' => 'schedule_nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Scheduled Programs'),
    ),
  );
  $data['node']['station_program_iid'] = array(
    'group' => t('Station Program'),
    'title' => t('Program Schedule Times'),
    'help' => t('All the times this program is scheduled.'),
    'real field' => 'nid',
//    'field' => array(
//      'handler' => 'views_handler_field_upload_fid',
//    ),
    'filter' => array(
      'handler' => 'station_schedule_handler_filter_schedule_item',
      'title' => t('Is scheduled'),
      'type' => 'yes-no',
      'label' => t('Program is scheduled'),
      'help' => t('Only display programs that are on a schedule. This can cause duplicates if the schedule is on multiple schedules.'),
    ),
    'relationship' => array(
      'title' => t('Program Scheduling'),
      'help' => t('Add a relationship to gain access to data about the times a program is scheduled. Note that this relationship will cause duplicate schedule nodes if the program is multiple slots or on multiple schedules.'),
      'relationship table' => 'node',
      'relationship field' => 'nid',
      'base' => 'station_schedule_item',
      'base field' => 'program_nid',
      'handler' => 'views_handler_relationship',
      'label' => t("Program's scheduling"),
    ),
  );
}