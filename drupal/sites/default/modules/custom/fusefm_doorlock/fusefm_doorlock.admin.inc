<?php

// $Id$

/**
* @file
* Administration page callbacks for the fusefm_doorlock module.
*/

/**
* Form builder. Configure fusefm_doorlock.
*
* @ingroup forms
* @see system_settings_form().
*/
function fusefm_doorlock_admin_settings() {
    // Pull a list of LDAP server configurations from ldapauth module
    $ldap_servers = array();
    $result = db_query("SELECT sid, name, status FROM {ldapauth} WHERE status = '1' ORDER BY sid");
    while ($row = db_fetch_object($result)) {
        $ldap_servers[$row->sid] = $row->name;
    }
    
    $form['fusefm_doorlock_ldapconfig'] = array(
        '#type' => 'select',
        '#title' => t('LDAP Server to use'),
        '#options' => $ldap_servers,
        '#default_value' => variable_get('fusefm_doorlock_ldapconfig', ''),
        '#description' => t('LDAP settings will be inherited from this configuration.'),
    );
    
    $form['fusefm_doorlock_onair'] = array(
        '#type' => 'checkbox',
        '#title' => t('Put door lock in on-air mode'),
        '#default_value' => variable_get('fusefm_doorlock_onair', 0),
        '#description' => t('This is an override and should only be used in an emergency.'),
    );
    
    // This needs to be moved to station_fusefm module
    $form['fusefm_onair'] = array(
        '#type' => 'checkbox',
        '#title' => t('Put site in on-air mode'),
        '#default_value' => variable_get('fusefm_onair', 0),
        '#description' => t('This is an override and should only be used in an emergency.'),
    );
    
    // This needs to be moved to station_fusefm module
    $schedules = array();
    foreach (station_get_schedules() as $nid => $schedule) {
        $schedules[$nid] = $schedule['title'];
    }
    $form['fusefm_next_broadcast'] = array(
        '#type' => 'select',
        '#title' => t('Next/Current Broadcast'),
        '#options' => $schedules,
        '#default_value' => variable_get('fusefm_next_broadcast', 0),
        '#description' => t('Choose which is the next/current broadcast period. Do not change this during a broadcast or horrible things may happen.'),
    );
    
    return system_settings_form($form);
}