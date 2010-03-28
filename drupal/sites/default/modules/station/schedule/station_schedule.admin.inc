<?php
// $Id: station_schedule.admin.inc,v 1.7 2009/06/04 16:19:44 drewish Exp $

/**
 * Admin settings form.
 */
function station_schedule_admin_settings() {
  $form['bc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backwards compatibility'),
  );
  $form['bc']['station_schedule_redirect_old_urls'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect old links'),
    '#default_value' => variable_get('station_schedule_redirect_old_urls', 0),
    '#description' => t("Checking this will ensure that the station module continues to redirect old <code>/schedule</code> and <code>/station/schedule</code> links."),
  );
  $schedules = array();
  foreach (station_get_schedules() as $nid => $schedule) {
    $schedules[$nid] = $schedule['title'];
  }
  $form['bc']['station_schedule_default'] = array(
    '#type' => 'select',
    '#title' => t('Default schedule'),
    '#default_value' => variable_get('station_schedule_default', 0),
    '#options' => $schedules,
    '#description' => t("This you select the schedule that will be used as the default for backwards compatibility."),
  );

  return system_settings_form($form);
}
