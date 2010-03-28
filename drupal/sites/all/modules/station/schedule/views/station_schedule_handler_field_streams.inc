<?php
// $Id: station_schedule_handler_field_streams.inc,v 1.1 2009/09/20 06:56:56 drewish Exp $

class station_schedule_handler_field_streams extends views_handler_field {
  function render($values) {
    $streams = unserialize($values->{$this->field_alias});
    if (is_array($streams)) {
      return theme('station_streams', $streams);
    }
  }
}
