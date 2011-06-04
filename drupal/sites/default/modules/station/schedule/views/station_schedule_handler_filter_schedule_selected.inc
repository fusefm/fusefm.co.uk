<?php
// $Id: station_schedule_handler_filter_schedule_selected.inc,v 1.1 2010/07/03 20:42:02 drewish Exp $

class station_schedule_handler_filter_schedule_selected extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
    $id = variable_get('station_schedule_default', 0);
    $this->query->add_where($this->options['group'], "$this->table_alias.nid " . (($this->value) ? '= ' : '!= ') . $id);
  }
}
