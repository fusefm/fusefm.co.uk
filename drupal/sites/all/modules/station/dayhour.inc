<?php

// $Id: dayhour.inc,v 1.22 2009/11/28 22:17:27 drewish Exp $

// There are 1440 minutes in a day (60minute * 24hours = 1day).
define('MINUTES_IN_DAY', 1440);
// There are 10080 minutes in a week (60minute * 24hours * 1day = 1week).
define('MINUTES_IN_WEEK', 10080);

/**
 * Turn a string into a valid hour.
 *
 * @param $hour
 *   String containing an hour. e.g. 0-24, 0-12 am/pm, noon, midnight
 * @return
 *   An integer between 0 and 23.
 */
function station_valid_hour($hour) {
  if (!is_numeric($hour)) {
    $hour = drupal_strtolower(trim($hour));

    // take care of a couple of handy strings
    if ($hour == t('midnight')) {
      return 0;
    }
    elseif ($hour == t('noon')) {
      return 12;
    }

    // try to parse out am/pm times then let it fall through to the
    // 24 hour stuff below.
    $parts = array();
    if (preg_match('/(\d+)\s*([ap]m)/', $hour, $parts)) {
      $hour = (integer) $parts[1];
      // 12am and 12pm are special cases
      if ($hour == 12) {
        $hour += 12;
      }
      if ($parts[2] == 'pm') {
        $hour += 12;
      }
    }
  }

  if ($hour < 0) {
    return ((integer) $hour % 24) + 24;
  }
  return ((integer) $hour % 24);
}

/**
 * Get a numeric day of the week from a string or integer.
 *
 * @param $day
 *   An integer from 0-6, or a case in-sensitive, English day name.
 * @return
 *   An nteger between 0 and 6 (Sunday, Monday, ... , Saturday).
 */
function station_valid_day($day) {
  if (is_numeric($day)) {
    if ($day < 0) {
      return ((integer) $day % 7) + 7;
    }
    elseif ($day > 6) {
      return ((integer) $day % 7);
    }
    return (integer) $day;
  }
  else {
    $dayname = station_day_name();
    $ret = array_search(drupal_ucfirst($day), $dayname, FALSE);
    return ($ret === FALSE) ? 0 : $ret;
  }
}

/**
 * Return an array of the names of the days of the week.
 *
 * @param $day
 *   An optional integer for the day of the week, zero being Sunday.
 * @return
 *   If $day is specified a string, othereise an array of the names of all
 *   the days in the week. When an array is returned it will be ordered
 *   honoring the site's first day of the week setting.
 */
function station_day_name($day = NULL) {
  static $days;

  if (!isset($days)) {
    $names = array(
      0 => t('Sunday'),
      t('Monday'),
      t('Tuesday'),
      t('Wednesday'),
      t('Thursday'),
      t('Friday'),
      t('Saturday')
    );
    $day_offset = variable_get('date_first_day', 0);
    $days = array();
    for ($i = $day_offset; $i < (7 + $day_offset); $i++) {
      $days[$i % 7] = $names[$i % 7];
    }
  }

  if ($day === NULL) {
    return $days;
  }
  else {
    return $days[$day % 7];
  }
}

/**
 * Given a day and hour, compute the number of minutes since midnight Sunday.
 *
 * @param $day
 *   Integer from 0 to 6.
 * @param $hour
 *   Integer from 0 to 23.
 * @return
 *   Integer specifying the number of minutes.
 */
function station_minute_from_day_hour($day, $hour) {
  return (($day * 24) + $hour) * 60;
}

/**
 * Convert a local timestamp into an integer specifying minutes since midnight
 * on Sunday.
 *
 * @return
 *   Integer specifying the number of minutes.
 *
 * @see station_local_ts()
 */
function station_minute_from_local_ts($local_timestamp = NULL) {
  if (!isset($local_timestamp)) {
    $local_timestamp = station_local_ts();
  }
  list($day, $hour, $minute) = explode(' ', date('w G i', $local_timestamp));
  return (($day * 24) + $hour) * 60 + $minute;
}

/**
 * Day of the week from minutes.
 *
 * @param $minutes
 *   Integer with the number of minutes since midnight Sunday.
 * @return
 *   Integer day of the week, zero being Sunday.
 */
function station_day_from_minute($minute) {
  return (int) (($minute) / MINUTES_IN_DAY);
}

/**
 * Compute time information for a minute in the week.
 *
 * @param $minutes
 *   Integer specifying minutes since midnight on Sunday.
 * @return
 *   An array with the following keys:
 *     'w'       - Day of week (0-6).
 *     'G'       - 24 hour.
 *     'g'       - 12 hour.
 *     'H'       - 24 hour, 0 padded.
 *     'h'       - 12 hour, 0 padded.
 *     'i'       - minutes, 0 padded.
 *     'time'    - hour and minutes acording to 12/24 setting.
 *     'minutes' - minutes since midnight Sunday.
 *     'a'       - am/pm.
 */
function station_time_from_minute($minutes) {
  $min = $minutes % 60;
  $day = (int) (($minutes) / MINUTES_IN_DAY);
  $hour24 = (int) (($minutes % MINUTES_IN_DAY) / 60);
  if (!($hour12 = $hour24 % 12)) {
    $hour12 = 12;
  }
  $i = str_pad($min, 2, '0', STR_PAD_LEFT);
  $h = str_pad($hour12, 2, '0', STR_PAD_LEFT);
  if (variable_get('station_clock', 12) == 12) {
    $time = $hour12 . (($min == 0) ? '' : ":$i");
    $a = ($hour24 > 11) ? 'pm' : 'am';
  }
  else {
    $time = "$hour24:$i";
    $a = '';
  }
  return array(
    'w' => $day,
    'G' => $hour24,
    'g' => $hour12,
    'H' => str_pad($hour24, 2, '0', STR_PAD_LEFT),
    'h' => $h,
    'i' => $i,
    'time' => $time,
    'minutes' => $minutes,
    'a' => $a,
  );
}

/**
 * Format minutes into a day hour string, e.g. "Sunday 11:15pm".
 *
 * @param $time
 *   Integer specifying minutes.
 * @return
 *   Formatted string.
 */
function theme_station_dayhour($time) {
  $time = station_time_from_minute($time);
  $format_params = array(
    '@day' => station_day_name($time['w']), '@hour' => $time['g'],
    '@time' => $time['time'], '@ampm' => $time['a'],
  );

  return t('@day @time@ampm', $format_params);
}

/**
 * Format a range of minutes into a day hour string, e.g.
 * "Sunday 11pm - Monday 1am".
 *
 * @param $start
 *   Integer specifying minutes.
 * @param $finish
 *   Integer specifying minutes.
 * @return
 *   Formatted string.
 */
function theme_station_dayhour_range($start, $finish) {
  $start = station_time_from_minute($start);
  $finish = station_time_from_minute($finish);
  $format_params = array(
    '@sday' => station_day_name($start['w']), '@shour' => $start['g'],
    '@stime' => $start['time'], '@sampm' => $start['a'],
    '@fday' => station_day_name($finish['w']), '@fhour' => $finish['g'],
    '@ftime' => $finish['time'], '@fampm' => $finish['a'],
  );

  // same day
  if ($start['w'] == $finish['w']) {
    // same am pm
    if ($start['a'] == $finish['a']) {
      return t('@sday @stime-@ftime@sampm', $format_params);
    }
    else {
      return t('@sday @stime@sampm-@ftime@fampm', $format_params);
    }
  }
  else {
    return t('@sday @stime@sampm-@fday @ftime@fampm', $format_params);
  }
}

/**
 * Format a range of minutes into a hour string, e.g. "1am-3pm".
 *
 * @param $start
 *   Integer specifying minutes.
 * @param $finish
 *   Integer specifying minutes.
 * @return
 *   Formatted string.
 */
function theme_station_hour_range($start, $finish) {
  $start = station_time_from_minute($start);
  $finish = station_time_from_minute($finish);
  $format_params = array(
    '@stime' => $start['time'], '@sampm' => $start['a'],
    '@ftime' => $finish['time'], '@fampm' => $finish['a'],
  );

  if ($start['a'] == $finish['a']) {
    return t('@stime-@ftime@sampm', $format_params);
  }
  else {
    return t('@stime@sampm-@ftime@fampm', $format_params);
  }
}

/**
 * Print the amount of time between start and finish.
 *
 * @param $start
 *   Integer specifying minutes.
 * @param $finish
 *   Integer specifying minutes.
 * @return
 *   Formatted string.
 */
function theme_station_hour_duration($start, $finish) {
  return format_interval(($finish - $start) * 60);
}

/**
 * Format minutes into a hour string, e.g. "1am".
 *
 * @param $time
 *   Integer specifying minutes.
 * @return
 *   Formatted string.
 */
function theme_station_hour($time) {
  $time = station_time_from_minute($time);
  $format_params = array(
    '@time' => $time['time'], '@ampm' => $time['a'],
  );

  return t('@time@ampm', $format_params);
}
