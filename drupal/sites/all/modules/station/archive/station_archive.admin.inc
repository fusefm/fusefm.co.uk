<?php
// $Id: station_archive.admin.inc,v 1.3 2009/11/28 22:06:44 drewish Exp $

/**
 * Archive settings form.
 */
function station_archive_admin_settings() {
  $times = array(86400, 172800, 259200, 345600, 432000, 518400, 604800, 1209600, 1814400, 2419200, 3024000, 3628800, 4233600, 4838400, 5443200, 6048000);
  $ageoptions = drupal_map_assoc($times, 'format_interval');

  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import'),
    '#collapsible' => FALSE,
  );
  $form['import']['station_archive_import_new'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import new audio'),
    '#default_value' => variable_get('station_archive_import_new', 1),
    '#description' => t("If this is checked, audio files left in the import directory will be added to the archive. The only reason to uncheck this is if you're running into a bug and want to stop imports until it can be fixed."),
  );
  $form['import']['station_archive_import_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('Import path'),
    '#default_value' => variable_get('station_archive_import_dir', drupal_get_path('module', 'station_archive') .'/import'),
    '#after_build' => array('system_check_directory'),
    '#required' => TRUE,
    '#description' => t('The directory where new MP3s are left by the stream ripper script. Drupal will need to have write access to this directory so we can move the file.'),
  );
  $form['import']['title_format']['station_archive_title_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Title format'),
    '#default_value' => variable_get('station_archive_title_format', '[audio-tag-title-raw]'),
    '#required' => TRUE,
    '#description' => t('This will be used as the title format for the audio nodes. You can use the tokens listed below to insert information into the title. <strong>Note:</strong> it is safe to use the -raw tokens.'),
  );
  $form['import']['title_format']['token_help'] = array(
    '#title' => t('Token list'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('This is a list of the tokens that can be used in the title of audio nodes.'),
    'help' => array('#value' => theme('token_help', 'node')),
  );
  $form['import']['scheduled'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scheduled'),
    '#collapsible' => FALSE,
    '#description' => t("These settings apply to audio recorded during a time when a program is scheduled. The program's information will be used to fill in the audio node's title, artist and genre tags."),
  );
  $form['import']['scheduled']['station_archive_promote_scheduled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Promote scheduled audio'),
    '#default_value' => variable_get('station_archive_promote_scheduled', 1),
    '#description' => t("If this is checked, audio nodes recorded during scheduled programs will be promoted to the front page."),
  );
  $form['import']['unscheduled'] = array(
    '#type' => 'fieldset',
    '#title' => t('Uncheduled'),
    '#collapsible' => FALSE,
    '#description' => t("These settings apply to audio recorded during a time when no program is scheduled."),
  );
  $form['import']['unscheduled']['station_archive_delete_unscheduled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Discard unscheduled audio'),
    '#default_value' => variable_get('station_archive_delete_unscheduled', 0),
    '#description' => t("If this is checked, any audio recorded during an unscheduled time will be deleted rather than imported."),
  );
  $form['import']['unscheduled']['station_archive_unscheduled_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Unscheduled audio title'),
    '#default_value' => variable_get('station_archive_unscheduled_title', t('DJ Auto mix')),
    '#required' => TRUE,
    '#description' => t("This value will be used for the audio node's title tag field when importing audio recorded during an unscheduled time."),
  );

  $form['cleanup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clean up'),
    '#collapsible' => FALSE,
  );
  $form['cleanup']['station_archive_cleanup_old'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove old audio'),
    '#default_value' => variable_get('station_archive_cleanup_old', 1),
    '#description' => t("If this is checked, audio that's older than the maximum age will be removed."),
  );
  $form['cleanup']['station_archive_max_age'] = array(
    '#type' => 'select',
    '#title' => t('Maximum age'),
    '#default_value' => variable_get('station_archive_max_age', 604800),
    '#options' => $ageoptions,
    '#description' => t("Audio that's been in the archive longer than this will be removed."),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
