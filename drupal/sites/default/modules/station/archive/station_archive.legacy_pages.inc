<?php
// $Id: station_archive.legacy_pages.inc,v 1.2 2009/11/28 22:06:44 drewish Exp $

function station_archive_view_html($dayname = NULL, $hourname = NULL) {
  if (!is_null($dayname)) {
    $day = station_valid_day($dayname);
    $vid = _station_archive_get_vid();
    if ($dayterm = _station_archive_get_day_term($vid, $day)) {
      $tid = $dayterm['tid'];

      if (!is_null($hourname)) {
        $hour = station_valid_hour($hourname);
        if ($hourterm = _station_archive_get_hour_term($dayterm, $hour)) {
          $tid = $hourterm['tid'];
        }
      }
      drupal_goto('taxonomy/term/'. $tid);
    }
  }
  drupal_not_found();
}

function station_archive_view_rss($dayname = NULL, $hourname = NULL) {
  if (!is_null($dayname)) {
    $day = station_valid_day($dayname);
    $vid = _station_archive_get_vid();
    if ($dayterm = _station_archive_get_day_term($vid, $day)) {
      $tid = $dayterm['tid'];

      if (!is_null($hourname)) {
        $hour = station_valid_hour($hourname);
        if ($hourterm = _station_archive_get_hour_term($dayterm, $hour)) {
          $tid = $hourterm['tid'];
        }
      }

      drupal_goto('taxonomy/term/'. $tid .'/0/feed');
    }
  }
  drupal_not_found();
}

function station_archive_view_hours_html($hour_string) {
  $title = t('Shows on...');
  $rss_url = url('rss/hours/'. str_replace(' ', '+', $hour_string));

  $attributes = array(
    'rel' => 'alternate',
    'type' => 'application/rss+xml',
    'title' => t('RSS - %title', array('%title' => $title)),
    'href' => $rss_url,
  );
  drupal_add_link($attributes);
  drupal_set_title($title);

  $vid = _station_archive_get_vid();
  $tids = _station_archive_get_hour_tids($vid, $hour_string);
  $result = taxonomy_select_nodes($tids, 'or', 0, FALSE);
  while ($node = db_fetch_object($result)) {
    $output .= node_view(node_load($node), 1);
  }
  $output .= theme('feed_icon', $rss_url);

  return $output;
}

function station_archive_view_hours_rss($hour_string) {
  $title = t('Shows on...');
  $desc = t('Description of shows...');

  $vid = _station_archive_get_vid();
  $tids = _station_archive_get_hour_tids($vid, $hour_string);
  $result = taxonomy_select_nodes($tids, 'or', 'all', FALSE);

  $channel['title'] = $title;
  $channel['description'] = $desc;

  return node_feed($result, $channel);
}