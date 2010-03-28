<?php
// $Id: station_schedule_handler_field_time_range.inc,v 1.3 2009/09/22 21:56:21 drewish Exp $

class station_schedule_handler_field_time_range extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['style'] = array('default' => 'station_hour_range');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['style'] = array(
      '#type' => 'radios',
      '#title' => t('Style'),
      '#options' => array(
        'station_hour_duration' => t('Duration'),
        'station_hour_range' => t('Hour range'),
        'station_dayhour_range' => t('Day/hour range'),
      ),
      '#default_value' => $this->options['style'],
      '#description' => t('Which time style should be used.'),
    );
  }

  function query() {
    // Since 'time' isn't a real field just add in our additional fields and
    // be done with it.
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if (isset($values->{$this->aliases['start']})) {
      return theme($this->options['style'], $values->{$this->aliases['start']}, $values->{$this->aliases['finish']});
    }
  }
}
