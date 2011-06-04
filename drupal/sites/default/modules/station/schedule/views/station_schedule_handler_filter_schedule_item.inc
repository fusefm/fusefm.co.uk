<?php
// $Id: station_schedule_handler_filter_schedule_item.inc,v 1.3 2010/09/14 05:28:05 timplunkett Exp $

class station_schedule_handler_filter_schedule_item extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "(SELECT COUNT(*) FROM {station_schedule_item} ssi WHERE ssi.$this->real_field = $this->table_alias.nid) " . (empty($this->value) ? '=' : '<>') . " 0");
  }
}
