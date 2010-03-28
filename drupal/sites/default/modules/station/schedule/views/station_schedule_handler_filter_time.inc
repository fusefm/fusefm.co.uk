<?php
// $Id: station_schedule_handler_filter_time.inc,v 1.3 2009/11/28 22:06:44 drewish Exp $

class station_schedule_handler_filter_time extends views_handler_filter_numeric {
  // Hide a bunch of the operators.
  function operators() {
    $operators = parent::operators();
    // BETWEEN doesn't really work with the 'now' mode.
    unset($operators['between'], $operators['not between']);
    // NULLs don't make sense since we require values for start and finish.
    unset($operators['empty'], $operators['not empty']);
    return $operators;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['value']['contains']['mode']['default'] = 'now';
    $options['value']['contains']['offset']['default'] = 0;
    return $options;
  }

  function value_form(&$form, &$form_state) {
    $form['value']['mode'] = array(
      '#type' => 'radios',
      '#title' => 'Mode',
      '#options' => array(
        'now' => t('Current time'),
        'today' => t('Current day'),
        'minutes' => t('Specific time'),
      ),
      '#default_value' => $this->value['mode'],
    );

    parent::value_form($form, $form_state);

    $form['value']['offset'] = array(
      '#type' => 'textfield',
      '#title' => empty($form_state['exposed']) ? t('Minute offset') : '',
      '#size' => 10,
      '#default_value' => $this->value['offset'],
      '#description' => t('Offset in minutes, e.g. -60 for one hour previous, 1440 for one day in the future.'),
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[value][mode]' => array('now', 'today')),
    );
    $form['value']['value'] = array(
      '#type' => 'station_schedule_daytime',
      '#title' => $form['value']['value']['#title'],
      '#default_value' => $this->value['value'],
      '#process' => array('expand_station_schedule_daytime', 'views_process_dependency'),
      '#dependency' => $form['value']['value']['#dependency'] + array('radio:options[value][mode]' => array('minutes')),
      '#dependency_count' => 2,
    );
  }

  function admin_summary() {
    $options = $this->operator_options('short');
    $output = check_plain($options[$this->operator]) . ' ';

    switch ($this->value['mode']) {
      case 'now':
        if (empty($this->value['offset'])) {
          $output .= t('Now');
        }
        elseif ($this->value['offset'] < 0) {
          $output .= t('Now -@offset', array('@offset' => format_interval($this->value['offset'] * 60)));
        }
        else {
          $output .= t('Now +@offset', array('@offset' => format_interval($this->value['offset'] * 60)));
        }
        break;

      case 'today':
        if (empty($this->value['offset'])) {
          $output .= t('Midnight today');
        }
        elseif ($this->value['offset'] < 0) {
          $output .= t('Midnight today -@offset', array('@offset' => format_interval($this->value['offset'] * 60)));
        }
        else {
          $output .= t('Midnight today +@offset', array('@offset' => format_interval($this->value['offset'] * 60)));
        }
        break;

      case 'minutes':
        $output .= theme('station_dayhour', $this->value['value']);
        break;

      default:
        $output .= check_plain((string) $this->value['value']);
    }
    return $output;
  }

  function query() {
    $this->ensure_my_table();

    $field = "$this->table_alias.$this->real_field";
    $offset = (int) $this->value['offset'];
    $min_in_day = MINUTES_IN_DAY;

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $formula = $field . ' ' . $this->operator . ' ';
      $params = array();
      $tokens = array(
        'now' => '***STATION_CURRENT_MINUTE***',
        'today' => '***STATION_CURRENT_DAY_IN_MINUTES***',
      );
      if (isset($tokens[$this->value['mode']])) {
        if (empty($offset)) {
          $formula .= $tokens[$this->value['mode']];
        }
        else {
          // If there's an offset we need to make sure that it wraps at the
          // ends of the week.
          $minutes_in_week = MINUTES_IN_WEEK;
          $formula .= "({$tokens[$this->value['mode']]} + %d + {$minutes_in_week}) %% {$minutes_in_week}";
          $params[] = $offset;
        }
      }
      elseif ('at' == $this->value['mode']) {
        $formula .= '%d';
        $params[] = $this->value['value'];
      }
      else {
        // Unknown.
        return;
      }
      $this->query->add_where($this->options['group'], $formula, $params);
    }
    return;
  }
}