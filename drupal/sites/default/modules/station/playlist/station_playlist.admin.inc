<?php
// $Id: station_playlist.admin.inc,v 1.3 2009/11/26 03:55:14 drewish Exp $

function station_playlist_admin_settings() {
  $form = array();
  $form['dates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date formats'),
    '#tree' => FALSE,
    '#description' => t("See PHP's <a href='!date-url'>date()</a> function for more information date formatting. No hour or minute infomation is collected, so it will not be displayed correctly if you attempt to use those in the format.", array('!date-url' => url('http://php.net/date'))),
  );
  $form['dates']['station_playlist_title_dateformat'] = array(
    '#type' => 'textfield',
    '#title' => t('Date format in playlist title'),
    '#default_value' => variable_get('station_playlist_title_dateformat', 'm/d/Y'),
    '#description' => t("The playlist's date is put into the title as a string. This setting lets you control how it is formatted."),
  );
  $form['dates']['station_playlist_program_dateformat'] = array(
    '#type' => 'textfield',
    '#title' => t('Date format on program node'),
    '#default_value' => variable_get('station_playlist_program_dateformat', 'F j, Y'),
    '#description' => t("The playlist's date is also displayed on the the program node. This setting lets you control how it is formatted."),
  );
  $form['tracks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tracks'),
    '#tree' => FALSE,
  );
  if (module_exists('station_catalog')) {
    $form['tracks']['station_playlist_track_autocomplete_source'] = array(
      '#type' => 'radios',
      '#title' => t('Auto-complete source'),
      '#default_value' => variable_get('station_playlist_track_autocomplete_source', 'playlists'),
      '#options' => array(
        'playlists' => t('Existing playlists'),
        'catalog'   => t('Albums in the catalog'),
        'both'      => t('Both'),
      ),
      '#description' => t("When adding playlists the track listings use auto-completing to save typing and provide suggestions. What data source should be used to provide these suggestions?"),
    );
  }
  else {
    $form['tracks']['station_playlist_track_autocomplete_source'] = array(
      '#type' => 'value',
      '#value' => 'playlists',
    );
  }

  return system_settings_form($form);
}
