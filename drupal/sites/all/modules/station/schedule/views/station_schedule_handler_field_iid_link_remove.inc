<?php
// $Id: station_schedule_handler_field_iid_link_remove.inc,v 1.2 2008/12/14 20:06:51 drewish Exp $
/**
 * Field handler to present a link to remove a schedule item.
 */
class station_schedule_handler_field_iid_link_remove extends station_schedule_handler_field_iid_link {
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('remove');
    $iid = $values->{$this->aliases['iid']};
    $schedule_nid = $values->{$this->aliases['schedule_nid']};
    return l($text, "node/$schedule_nid/schedule/$iid/remove", array('query' => drupal_get_destination()));
  }
}

