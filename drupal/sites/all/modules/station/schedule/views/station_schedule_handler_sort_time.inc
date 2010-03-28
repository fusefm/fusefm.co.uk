<?php
// $Id: station_schedule_handler_sort_time.inc,v 1.4 2009/09/22 23:47:56 drewish Exp $

class station_schedule_handler_sort_time extends views_handler_sort {
  function option_definition() {
    $options = parent::option_definition();
    $options['start_from'] = array('default' => 'begin_week');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['start_from'] = array(
      '#type' => 'select',
      '#title' => t('Start from'),
      '#options' => array('begin_week' => t('Beginning of the week'), 'now' => t('Now')),
      '#default_value' => $this->options['start_from'],
    );
  }

  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->ensure_my_table();

    $minutes_in_day = MINUTES_IN_DAY;
    $minutes_in_week = MINUTES_IN_WEEK;

    switch ($this->options['start_from']) {
      case 'begin_week':
        $formula = "$this->table_alias.$this->real_field";
        $first_day = 7 - variable_get('date_first_day', 0);
        if ($first_day != 0) {
          $formula = "($formula + ($first_day * $minutes_in_day)) %% $minutes_in_week";
        }
        break;

      case 'now':
        $formula = "(($this->table_alias.$this->real_field - ***STATION_CURRENT_MINUTE***) + $minutes_in_week) %% $minutes_in_week";
        break;

      default:
        return;
    }

    $this->query->add_orderby(NULL, $formula, $this->options['order'], $this->table_alias .'_'. $this->field .'_sort');
  }
}
