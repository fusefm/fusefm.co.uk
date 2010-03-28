#!/usr/local/bin/php -q
<?php

// $Id: ripper.php,v 1.10 2009/11/28 22:17:27 drewish Exp $

/**
 * This script is designed to be run by a cron job or windows scheduled task.
 * It connects to a webstream, downloads one hours worth and saves it as an
 * MP3 for the station archive module to import.
 *
 *     Y O U   D O N ' T   N E E D   T O   E D I T   T H I S   F I L E !
 *       Configuration options are now set in the ripper.inc file.
 *
 * This script requires that Streamripper (http://streamripper.sourceforge.net/)
 * version 1.61.17 or higher be installed (we use the --quite option)
 */

// Make sure we're being run from the command line.
if (isset($_SERVER['REMOTE_ADDR'])) {
  exit("This script must be called from the command line.\n");
}

// Load our settings from the ripper.inc file.
$incfile = realpath(dirname(__FILE__) .'/ripper.inc');
if (!file_exists($incfile)) {
  exit("Cannot read the ripper.inc file from {$incfile}.\n");
}
$settings = parse_ini_file($incfile);

// Check that the import directory is writable.
$import_dir = realpath($settings['import_path']);
if (!is_dir($import_dir) || !is_writable($import_dir)) {
  exit("Cannot write to the import directory '{$import_dir}'.\n");
}

// Make sure we can find stream ripper. The is_executable() test might not work
// with PHP4 and Windows. Upgrade! PHP 5.1 is great.
$streamripper = realpath($settings['streamripper_path']);
if (!file_exists($streamripper) || !is_executable($streamripper)) {
  exit("Couldn't find the stream ripper executable at '{$streamripper}'.\n");
}

// Determine when we're starting, when we should end and convert that to a
// length of time in seconds.
$start_time = round_to_nearest_hour(time());
$end_time = $start_time + 3600;
$length = ($end_time - time()) + (int) $settings['overlap_seconds'];

// Download the stream
$stream_url = $settings['stream_url'];
$file_format = strtolower($settings['file_format']);
exec("{$streamripper} {$stream_url} -s -d {$import_dir} -A -l {$length} -a {$start_time}.{$file_format} --quiet");

// stream ripper creates the .cue file. we'll use its absence as a signal
// to the module that it's safe to import a file.
$cuefile = "{$import_dir}/{$start_time}.cue";
if (file_exists($cuefile)) {
  unlink($cuefile);
}

exit(0);


/**
 * Round a timestamp to the nearest hour.
 *
 * @param $time
 *   A UNIX timestamp
 */
function round_to_nearest_hour($time) {
  $parts = getdate($time);
  if ($parts['minutes'] > 50) {
    // advance it to the next hour
    return mktime($parts['hours'] + 1, 0, 0, $parts['mon'], $parts['mday'], $parts['year']);
  }
  else {
    // we're late for this hour
    return mktime($parts['hours'], 0, 0, $parts['mon'], $parts['mday'], $parts['year']);
  }
}

