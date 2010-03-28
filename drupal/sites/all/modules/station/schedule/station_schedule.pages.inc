<?php
// $Id: station_schedule.pages.inc,v 1.8 2009/11/28 23:26:57 drewish Exp $

/**
 * Redirect the visitor to the schedule's current day view.
 */
function station_schedule_goto_today($schedule) {
  drupal_goto('node/'. $schedule->nid .'/view/'. strtolower(station_day_name(date('w'))));
}

function theme_station_schedule_hour($hour) {
  $class = 'station-sch-box station-sch-hour';
  $height = 60;
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= theme('station_hour', $hour * 60);
  $output .= "</div>\n";
  return $output;
}

function theme_station_schedule_spacer($start, $finish) {
  $class = 'station-sch-box station-sch-unscheduled';
  $height = ($finish - $start);
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= "<span class='station-sch-time'>". theme('station_hour_range', $start, $finish) ."</span>";
  $output .= "</div>\n";
  return $output;
}

function theme_station_schedule_item($start, $finish, $program) {
  $class = 'station-sch-box station-sch-scheduled';
  $height = ($finish - $start);
  $link = url('node/'. $program->nid);

  $output = "<div class='{$class}'><a href='{$link}' style='height: {$height}px;'>";
  $output .= '<span class="station-sch-time">'. theme('station_hour_range', $start, $finish) .'</span>';
  $output .= '<span class="station-sch-title">'. check_plain($program->title) .'</span>';
  if (!empty($program->field_station_program_dj)) {
    $djs = array();
    foreach ($program->field_station_program_dj as $entry) {
      $user = user_load($entry);
      $djs[] .= $user->profile_givenName . ' ' . $user->profile_sn;
    }
    $output .= '<span class="station-sch-djs">'. check_plain(implode(', ', $djs)) .'</span>';
  }
  $output .= "</a></div>\n";
  return $output;
}

/**
 * Print a day's schedule page.
 */
function station_schedule_day_page($node, $day) {
  if ($view = views_get_view('station_schedule_day')) {
    $display_id = 'default';
    if ($view->access($display_id)) {
      return $view->execute_display($display_id, array($node->nid, $day));
    }
    $view->destroy();
  }
  return drupal_not_found();
}

