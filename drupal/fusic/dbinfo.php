<?php
/*
dbinfo.php
Fuse Playout System Web Content aka FusicBrainz
This file contains the details for communication with the MySQL server.
*/

// MySQL Server Details
$mysqlserver = "localhost";
$mysqlport = "3306";
$mysqluser = "playoutsystem";
$mysqlpass = "vWV2nxc9xQE4y4TU";
$mysqldb = "playoutsystem";


// DO NOT EDIT BELOW THIS LINE...

// MySQL connection
if ($mysqlport == "") {
  $mysqlserver = $mysqlserver . ":3306";
} else {
  $mysqlserver = $mysqlserver . ":" . $mysqlport;
}

$mysqllink = mysql_connect($mysqlserver, $mysqluser, $mysqlpass);
if (!$mysqllink) {
    die('Not connected to MySQL server: ' . mysql_error());
}

$db_selected = mysql_select_db($mysqldb, $mysqllink);
if (!$db_selected) {
    die ('Database specified cannot be found or read: ' . mysql_error());
}

?>
