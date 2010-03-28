<?php
// $Id: station_schedule_handler_argument_day.inc,v 1.2 2009/09/21 21:36:20 drewish Exp $

/**
 * @file
 * Views argument handler for Image size argument.
 */

/**
 * Argument handler for the schedule item's day of the week.
 */
class station_schedule_handler_argument_day extends views_handler_argument {
  function title() {
    return station_day_name(station_valid_day($this->argument));
  }

  /**
   * Build the summary query based on a formula
   */
  function summary_query() {
    $this->ensure_my_table();

    // Convert the Group the programs by day.
    $min_in_day = MINUTES_IN_DAY;
    $formula = "CAST($this->table_alias.start / $min_in_day AS UNSIGNED INTEGER) %% 7";

    // Add the field.
    $suggested_alias = $this->table_alias .'_start_group';
    $this->base_alias = $this->name_alias = $this->query->add_field(NULL, $formula, $suggested_alias);
    $this->query->set_count_field(NULL, $formula, $suggested_alias);

    return $this->summary_basics(FALSE);
  }

  function summary_name($data) {
    return station_day_name($data->{$this->name_alias});
  }

  /**
   * Build the query based upon the formula
   */
  function query() {
    $this->ensure_my_table();

    $day = station_valid_day($this->argument);
    $start = $day * MINUTES_IN_DAY;
    $finish = ($day + 1) * MINUTES_IN_DAY;

    // The idea is that we want all the items that end after the day begins or
    // start before the day ends.
    $formula = "$this->table_alias.finish > %d AND $this->table_alias.start < %d";
    $this->query->add_where(0, $formula, $start, $finish);
  }
}
