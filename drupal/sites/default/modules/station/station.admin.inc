<?php

// $Id: station.admin.inc,v 1.2 2009/06/04 16:05:03 drewish Exp $

/**
 * Admin settings form.
 */
function station_admin_settings() {
  $form['station_clock'] = array(
    '#type' => 'radios',
    '#title' => t('Time format'),
    '#options' => array(12 => t('12-hour (am/pm)'), 24 => t('24-hour')),
    '#default_value' => variable_get('station_clock', 12),
    '#description' => t('This setting determines whether times are formatted using a 12 or 24-hour clock.'),
  );

  $form['station_remote'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sub-module Integration'),
    '#description' => t('The Station module is designed to allow different modules to be run on different servers. So the archive could be on a dedicated machine while the schedule runs on your primary webserver.'),
  );

  if (!module_exists('station_archive')) {
    $form['station_remote']['station_remote_archive_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Station Archive remote URL'),
      '#default_value' => variable_get('station_remote_archive_url', ''),
      '#description' => t("The archive module is not enabled on this machine. If you have it installed up on a different Drupal server, enter the URL here. If this is blank no archive links will be provided. Please include the trailing slash: i.e. http://example.com/"),
    );
  }
  else {
    $form['station_remote']['archive_status'] = array(
      '#type' => 'item',
      '#title' => t('Station Archive module is enabled'),
      '#value' => t("The Station Archive module is enabled on this machine. This can be used by the Station Schedule module to provide users links to a program's archived audio."),
    );
  }

  if (!module_exists('station_schedule')) {
    $form['station_remote']['station_remote_schedule_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Station Schedule remote URL'),
      '#default_value' => variable_get('station_remote_schedule_url', ''),
      '#description' => t("The station schedule module is not enabled on this machine. The Station module can connect to another Drupal server running the station schedule module and retrieve information on scheduled programs via XML-RPC. If this is left blank, no connection will be attempted but things may not work. Omit the trailing slash, i.e.: http://example.com or http://example.com/drupal"),
    );
    if ($schedules = station_get_schedules()) {
      $options = array();
      foreach (station_get_schedules() as $nid => $schedule) {
        $options[$nid] = $schedule['title'];
      }
      $form['station_remote']['station_remote_schedule_nid'] = array(
        '#type' => 'select',
        '#title' => t('Schedule'),
        '#default_value' => variable_get('station_remote_schedule_nid', 0),
        '#options' => $options,
        '#description' => t("This you select the schedule that will be used as the default for backwards compatibility."),
      );
    }
    $form['station_remote']['station_remote_schedule_offset'] = array(
      '#type' => 'textfield',
      '#title' => t('Station Schedule remote minute adjustment'),
      '#default_value' => variable_get('station_remote_schedule_offset', 0),
      '#description' => t("If the clocks on your two servers aren't accurately synchronized the archive will end up with the wrong names for programs. This number of minutes (positive or negative) will be added to the time before checking the schedule."),
    );
  }
  else {
    $form['station_remote']['schedule_status'] = array(
      '#type' => 'item',
      '#title' => t('Station Schedule module is enabled'),
      '#value' => t('The Station Schedule module is enabled on this machine. This information can be used by the Now Playing block and Station Archive module.'),
    );
  }

  return system_settings_form($form);
}

/**
 * Checks that the RPC endpoint is valid.
 */
function station_admin_settings_validate($form, &$form_state) {
  if (!module_exists('station_schedule')) {
    // check station url
    if ($url = $form_state['values']['station_remote_schedule_url']) {
      $url = check_url($url .'/xmlrpc.php');
      $ret = xmlrpc($url, 'station.program.get.at', time(), 0);
      if (xmlrpc_error_msg()) {
        form_set_error('station_remote_schedule_url',
          t('You must provide a a valid URL for a Drupal site with the station schedule module installed.<br />Specific error: %xmlrpc-error', array('%xmlrpc-error' => xmlrpc_error_msg()))
        );
      }
    }

    // check that the offset is an integer
    $offset = $form_state['values']['station_remote_schedule_offset'];
    if (!(is_numeric($offset) && intval($offset) == $offset)) {
      form_set_error('station_remote_schedule_offset', t('The offset must be an integer.'));
    }
  }
}