<?php
// $Id: station_schedule.edit.inc,v 1.4 2009/09/17 23:13:43 drewish Exp $

function station_schedule_item_list($node) {
  drupal_add_css(drupal_get_path('module', 'station_schedule') .'/station_schedule.css');
  drupal_set_title(check_plain($node->title));

  $header = array();
  $row = array();
  foreach ($node->schedule as $day => $items) {
    $header[$day] = station_day_name($day);
    $row[$day] = '';

    // The last finish pointer starts at the beginning of the day.
    $last_finish = $day * MINUTES_IN_DAY;
    $day_finish = ($day + 1) * MINUTES_IN_DAY;
    foreach ($items as $item) {
      // Display blocks for unscheduled time periods
      if ($last_finish != $item->start) {
        $row[$day] .= theme('station_schedule_admin_nonitem', $node, $last_finish, $item->start);
      }
      $last_finish = $item->finish;

      // Display the schedule item
      $item->program = node_load($item->program_nid);
      $row[$day] .= theme('station_schedule_admin_item', $node, $item);
    }
    // Display a block for any remaining time during the day.
    if ($last_finish < $day_finish) {
      $row[$day] .= theme('station_schedule_admin_nonitem', $node, $last_finish, $day_finish);
    }
  }

  // Render the table
  return theme('table', $header, array($row), array('id' => 'station-sch', 'class' => 'station-sch-admin'));
}

function theme_station_schedule_admin_nonitem($node, $start, $finish) {
  $class = 'station-sch-box station-sch-unscheduled';
  $height = ($finish - $start);
  $link = url("node/{$node->nid}/schedule/add/0/{$start}/{$finish}");

  $output = "<div class='{$class}'><a id='schedule-{$start}' href='{$link}' style='height: {$height}px;'>";
  $output .= '<span class="station-sch-time">'. theme('station_hour_range', $start, $finish) .'</span>';
  $output .= '<span class="station-sch-title">'. t('<em>Unscheduled</em>') .'</span>';
  $output .= "</a></div>\n";

  return $output;
}

function theme_station_schedule_admin_item($node, $item) {
  $class = 'station-sch-box station-sch-scheduled';
  $height = ($item->finish - $item->start);
  $link = url("node/{$node->nid}/schedule/{$item->iid}/edit");

  $output = "<div class='{$class}'><a id='schedule-{$item->start}' href='{$link}' style='height: {$height}px;'>";
  $output .= '<span class="station-sch-time">'. theme('station_hour_range', $item->start, $item->finish) .'</span>';
  $output .= '<span class="station-sch-title">'. check_plain($item->program->title) .'</span>';
  $output .= "</a></div>\n";

  return $output;
}

function station_schedule_item_add($schedule, $program_id = NULL, $start = 0, $finish = 60) {
  drupal_set_title(t('Add item to %title', array('%title' => $schedule->title)));

  $schedule_item = new stdClass;
  $schedule_item->schedule_nid = $schedule->nid;
  $schedule_item->iid = NULL;
  $schedule_item->program_nid = $program_id;
  $schedule_item->start = (int) $start;
  $schedule_item->finish = (int) $finish;
  $schedule_item->may_archive = TRUE;

  return drupal_get_form('station_schedule_item_edit_form', $schedule, $schedule_item, $start, $finish);
}

function station_schedule_item_edit($schedule, $schedule_item = NULL, $start = 0, $finish = 60) {
  drupal_set_title(t('Edit %title item', array('%title' => $schedule->title)));

  return drupal_get_form('station_schedule_item_edit_form', $schedule, $schedule_item, $start, $finish);
}

function station_schedule_item_edit_form(&$form_state, $schedule, $schedule_item = NULL, $start = 0, $finish = 60) {
  if (isset($form_state['post']['op']) && $form_state['post']['op'] == t('Cancel')) {
    drupal_goto("node/{$schedule->nid}/schedule", NULL, "schedule-{$start}");
  }
  $program_title = '';
  if (!empty($schedule_item->program_nid) && $program = node_load($schedule_item->program_nid)) {
    $program_title = $program->title;
  }

  $form['old_schedule_item'] = array(
    '#type' => 'value',
    '#value' => $schedule_item,
  );
  $form['schedule_nid'] = array(
    '#type' => 'value',
    '#value' => $schedule->nid,
  );
  $form['iid'] = array(
    '#type' => 'value',
    '#value' => $schedule_item->iid,
  );
  $form['range'] = array(
    '#type' => 'station_schedule_daytime_range',
    '#default_value' => array(
      'start' => $schedule_item->start,
      'finish' => $schedule_item->finish
    ),
    '#increment' => $schedule->settings['increment'],
  );
  $form['program_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Program'),
    '#description' => t("Enter the name of the program."),
    '#default_value' => $program_title,
    '#maxlength' => 128,
    '#autocomplete_path' => 'station/autocomplete/program',
  );
  $form['may_archive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Can be saved in Station Archive'),
    '#default_value' => $schedule_item->may_archive,
    '#description' => t('Checking this indicates that the Station Archive module can save audio recordings of the program at this timeslot to the archive.'),
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($schedule_item->iid) {
    $form['buttons']['remove'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => array('station_schedule_item_edit_remove_submit'),
    );
  }
  $form['buttons']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Button sumit function: handle the 'Remove' button on the item edit form.
 */
function station_schedule_item_edit_remove_submit($form, &$form_state) {
  $destination = '';
  if (isset($_REQUEST['destination'])) {
    $destination = drupal_get_destination();
    unset($_REQUEST['destination']);
  }
  $form_state['redirect'] = array('node/'. $form_state['values']['schedule_nid'] .'/schedule/'. $form_state['values']['iid'] .'/remove', $destination);
}

function station_schedule_item_edit_form_validate($form, &$form_state) {
  $start = $form_state['values']['range']['start'];
  $finish = $form_state['values']['range']['finish'];

  $result = db_query('SELECT count(*) AS count, min(s.start) AS start, max(s.finish) AS finish FROM {station_schedule_item} s WHERE s.schedule_nid = %d AND s.iid <> %d AND s.finish > %d AND s.start < %d', $form_state['values']['schedule_nid'], $form_state['values']['iid'], $start, $finish);
  if ($overlap = db_fetch_object($result)) {
    if ($overlap->count == 1) {
      form_set_error('', t("The program overlaps another scheduled item at %time.", array('%time' => theme('station_dayhour_range', $overlap->start, $overlap->finish))));
    }
    elseif ($overlap->count > 1) {
      form_set_error('', t("The program overlaps %number scheduled items from %time.", array('%time' => theme('station_dayhour_range', $overlap->start, $overlap->finish), '%number' => $overlap->count)));
    }
  }

  // check for a valid program title
  if (!$prog = node_load(array('title' => $form_state['values']['program_title'], 'type' => 'station_program'))) {
    form_set_error('program_title', t('You need to specify a valid program name.'));
  }
}

function station_schedule_item_edit_form_submit($form, &$form_state) {
  $program = node_load(array('type' => 'station_program', 'title' => $form_state['values']['program_title']));
  $schedule = node_load($form_state['values']['schedule_nid']);

  $record = array(
    'iid' => $form_state['values']['iid'],
    'schedule_nid' => $form_state['values']['schedule_nid'],
    'program_nid' => $program->nid,
    'start' => $form_state['values']['range']['start'],
    'finish' => $form_state['values']['range']['finish'],
    'may_archive' => $form_state['values']['may_archive'],
  );

  if ($form_state['values']['iid']) {
    drupal_write_record('station_schedule_item', $record, array('iid'));
    $old_record = (array) $form_state['values']['old_schedule_item'];
    if ($old_record['program_nid'] != $record['program_nid']) {
      _station_send_notice('schedule', 'remove', $old_record);
      _station_send_notice('schedule', 'add', $record);
    }
    elseif ($old_record['start'] != $record['start'] || $old_record['finish'] != $record['finish']) {
      _station_send_notice('schedule', 'change', $record);
    }
    drupal_set_message(t('%program was updated on the %schedule schedule.', array('%program' => $program->title, '%schedule' => $schedule->title)));
  }
  else {
    drupal_write_record('station_schedule_item', $record);
    _station_send_notice('schedule', 'add', $record);
    drupal_set_message(t('%program was added to the %schedule schedule.', array('%program' => $program->title, '%schedule' => $schedule->title)));
  }
  $form_state['redirect'] = array('node/'. $record['schedule_nid'] .'/schedule', NULL, 'schedule-'. $record['start']);
}

/**
 * Page to confirm the removal of a schedule item.
 */
function station_schedule_item_remove_form(&$form_state, $schedule, $schedule_item) {
  $program = node_load($schedule_item->program_nid);

  $form['schedule_nid'] = array('#type' => 'value', '#value' => $schedule->nid);
  $form['program_nid'] = array('#type' => 'value', '#value' => $schedule_item->program_nid);
  $form['iid'] = array('#type' => 'value', '#value' => $schedule_item->iid);
  return confirm_form(
    $form,
    t('Are you sure you want to unschedule %title from %schedule at %time?', array('%title' => $program->title, '%schedule' => $schedule->title, '%time' => theme('station_dayhour_range', $schedule_item->start, $schedule_item->finish))),
    isset($_GET['destination']) ? $_GET['destination'] : "node/{$schedule->nid}/schedule/{$schedule_item->iid}/edit",
    t('This action cannot be undone, but you can always re-add the program to the schedule.'),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Delete a schedule item.
 */
function station_schedule_item_remove_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_query("DELETE FROM {station_schedule_item} WHERE iid = %d", $form_state['values']['iid']);
    _station_send_notice('schedule', 'remove', array('schedule_nid' => $form_state['values']['schedule_nid'], 'program_nid' => $form_state['values']['program_nid'], 'iid' => $form_state['values']['iid']));
    $form_state['redirect'] = 'node/'. $form_state['values']['schedule_nid'] .'/schedule';
  }
}
