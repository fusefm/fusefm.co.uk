<?php
// $Id: station_schedule_handler_field_iid_link.inc,v 1.2 2009/04/19 21:34:12 drewish Exp $
/**
 * Field handler to present a link to the user.
 */
class station_schedule_handler_field_iid_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['iid'] = 'iid';
    $this->additional_fields['schedule_nid'] = 'schedule_nid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function access() {
// @TODO figure out a more granular way to handle permissions here.
    return user_access('administer station schedule');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    return '';

    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $uid = $values->{$this->aliases['uid']};
    return l($text, "user/$uid");
  }
}

