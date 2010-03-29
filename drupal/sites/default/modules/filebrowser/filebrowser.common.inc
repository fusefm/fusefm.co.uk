<?php

/* This file is part of "filebrowser".
 *    Copyright 2009, arNuméral
 *    Author : Yoran Brault
 *    eMail  : yoran.brault@bad_arnumeral.fr (remove bad_ before sending an email)
 *    Site   : http://www.arnumeral.fr
 *
 * "filebrowser" is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * "filebrowser" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with "filebrowser"; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

define('FILEBROWSER_CREATE_DIRECTORY_LISTING', 'create directory listings');
define('FILEBROWSER_DELETE_OWN_DIRECTORY_LISTINGS', 'delete own directory listings');
define('FILEBROWSER_DELETE_ANY_DIRECTORY_LISTINGS', 'delete any directory listings');
define('FILEBROWSER_EDIT_OWN_DIRECTORY_LISTINGS', 'edit own directory listings');
define('FILEBROWSER_EDIT_ANY_DIRECORY_LISTINGS', 'edit any directory listings');
define('FILEBROWSER_VIEW_DIRECORY_LISTINGS', 'view directory listings');
define('FILEBROWSER_UPLOAD', 'upload files');
define('FILEBROWSER_DOWNLOAD_ARCHIVE', 'download archive files');
define('FILEBROWSER_DOWNLOAD', 'download files');

/**
 * Column indentifiers definition.
 */
define('FILEBROWSER_DATA_NAME_ICON', 'icon');
define('FILEBROWSER_DATA_NAME_DISPLAY_NAME', 'display_name');
define('FILEBROWSER_DATA_NAME_SIZE', 'size');
define('FILEBROWSER_DATA_NAME_CREATED', 'created');
define('FILEBROWSER_DATA_NAME_MODIFIED', 'modified');
define('FILEBROWSER_DATA_NAME_TYPE', 'type');
define('FILEBROWSER_DATA_NAME_DESCRIPTION', 'description');

/**
 * Compare two sizes (used as usort callback).
 */
function _filebrowser_sorter_size($a, $b) {
   if ($a['size'] == $b['size']) {
      return 0;
   }
   return ($a['size'] > $b['size']) ? -1 : 1;
}

/**
 * Compare two creation dates (used as usort callback).
 */
function _filebrowser_sorter_created($a, $b) {
   if ($a['created'] == $b['created']) {
      return 0;
   }
   return ($a['created'] > $b['created']) ? -1 : 1;
}

/**
 * Compare two modification dates (used as usort callback).
 */
function _filebrowser_sorter_modified($a, $b) {
   if ($a['modified'] == $b['modified']) {
      return 0;
   }
   return ($a['modified'] > $b['modified']) ? -1 : 1;
}

/**
 * Compare two display names (used as usort callback).
 */
function _filebrowser_sorter_display_name($a, $b) {
   return -strcasecmp($a['display-name'], $b['display-name']);
}

/**
 * Compare file types (used as usort callback).
 */
function _filebrowser_sorter_type($a, $b) {
   return -strcasecmp($a['mime-type'], $b['mime-type']);
}

/**
 * Compare two creation descriptions (used as usort callback).
 */
function _filebrowser_sorter_description($a, $b) {
   return -strcasecmp($a['description'], $b['description']);
}

/**  Check the end of a string
 * @param $str source string
 * @param $sub element to search
 * @return return true is a string ends with another string.
 */
function _filebrowser_ends_with($str, $sub) {
   return (substr($str, strlen($str) - strlen($sub)) == $sub);
}

/** Helper to get the full path of a dir_listing node
 * @param $node source node
 * @return the full path
 */
function _filebrowser_current_full_path($node) {
   return _filebrowser_get_node_file_path($node) . $node->relative_path;
}

function filebrowser_configuration() {
   static $conf = null;
   
   if (!$conf) {
      $conf = variable_get("filebrowser", array ());
   }
   return $conf;
}

function filebrowser_is_debug() {
   static $is_debug = null;
   if ($is_debug == null) {
      $configuration = filebrowser_configuration();
      $is_debug = $configuration['debug'];
   }
   return $is_debug;
}

function filebrowser_debug($message) {
   static $is_debug = null;
   if ($is_debug == null) {
      $is_debug = filebrowser_is_debug();
   }
   if ($is_debug) {
      error_log("[filebrowser]" . $message);
   }
}


/**
 * Helper function to get node folder with correct token replacements. Never
 * use $node->folder_path directly !!!
 * @param $node
 * @return folder path
 */
function _filebrowser_get_node_file_path(& $node) {
   $result = $node->folder_path;
   if (module_exists("token")) {
      $result = token_replace($result, $type = 'global', $object = NULL, $leading = '[', $trailing = ']');
   }
   return $result;
}

function _filebrowser_prepare_record(& $node) {
   // Fix trailing slashes
   $node->folder_path = rtrim($node->folder_path, '/');
   
   // Convert array
   if (!is_object($node->folder_rights)) {
      $node->folder_rights = (object)$node->folder_rights;
   }
   $data = new stdclass();
   $data->folder_rights = $node->folder_rights;
   
   if (!is_object($node->folder_presentation)) {
      $node->folder_presentation = (object)$node->folder_presentation;
   }
   $data->folder_presentation = $node->folder_presentation;
   
   if (!is_object($node->folder_uploads)) {
      $node->folder_uploads = (object)$node->folder_uploads;
   }
   $data->folder_uploads = $node->folder_uploads;
   
   foreach (module_implements("filebrowser_handler_info") as $module) {
      if ($node->$module && !is_object($node->$module)) {
         $node->$module = (object)$node->$module;
      }
   }
   
   // If no column is visible, make at least defaults ICON+DISPLAY_NAME visible
   $node->folder_presentation->visible_columns = _filebrowser_filter_checkboxes_result($node->folder_presentation->visible_columns);
   
   if (!$node->folder_presentation->visible_columns || count($node->folder_presentation->visible_columns) == 0) {
      $node->folder_presentation->visible_columns = array (
            FILEBROWSER_DATA_NAME_ICON => 1, 
            FILEBROWSER_DATA_NAME_DISPLAY_NAME => 1 
      );
   }
   
   if (!isset($node->folder_presentation->default_sort)) {
      $node->folder_presentation->default_sort = FILEBROWSER_DATA_NAME_DISPLAY_NAME;
   }
   $node->folder_presentation->visible_columns[$node->folder_presentation->default_sort] = 1;
   
   if (!isset($node->folder_presentation->default_sort_order)) {
      $node->folder_presentation->default_sort_order = 'asc';
   }
   
   // Create serialized properties
   $data = new stdClass();
   $data->folder_rights = $node->folder_rights;
   $data->folder_presentation = $node->folder_presentation;
   $data->folder_uploads = $node->folder_uploads;
   foreach (module_implements("filebrowser_handler_info") as $module) {
      if ($node->$module) {
         $data->$module = $node->$module;
      }
   }
   $node->properties = serialize($data);
}

/**
 * Returns the appropriate HTML code for an icon representing
 * a file, based on the extension of the file. A specific icon
 * can also be requested with the second parameter.
 */
function _filebrowser_get_default_thumbnail($file) {
   $main_type = dirname($file['mime-type']);
   $mime_type = str_replace("/", "-", $file['mime-type']);
   $module_path = drupal_get_path("module", "filebrowser") . "/images/mime-types/";
   //FIXME ça ne marche pas, la fonction path_to_theme ne renvoie pas le bon chemin...
   $theme_path = path_to_theme() . "/filebrowser/";
   $icons = array (
         $theme_path . $mime_type . ".png", 
         $theme_path . $main_type . ".png", 
         $module_path . $mime_type . ".png", 
         $module_path . $main_type . ".png" 
   );
   foreach ($icons as $icon) {
      if (file_exists($icon)) {
         return theme('image', $icon);
      }
   }
   return theme('image', $module_path . "unknown.png");
}

function &_filebrowser_columns() {
   static $columns = null;
   if (!$columns) {
      $columns = array (
            FILEBROWSER_DATA_NAME_ICON => t('Icon'), 
            FILEBROWSER_DATA_NAME_DISPLAY_NAME => t('Display name'), 
            FILEBROWSER_DATA_NAME_SIZE => t('Size'), 
            FILEBROWSER_DATA_NAME_CREATED => t('Creation Time'), 
            FILEBROWSER_DATA_NAME_MODIFIED => t('Modification Time'), 
            FILEBROWSER_DATA_NAME_TYPE => t('Mime type'), 
            FILEBROWSER_DATA_NAME_DESCRIPTION => t('Description') 
      );
   }
   return $columns;
}

function _filebrowser_options_to_query($options, $values = array ()) {
   $options = array_merge($options, $values);
   unset($options['relative_path']);
   $query = '';
   foreach ($options as $key => $value) {
      if (!empty($query)) {
         $query .= "&";
      }
      $query .= "$key=$value";
   }
   return $query;
}

function _filebrowser_query_to_options(&$node) {
   static $options = array (
         'fid', 
         'sort', 
         'order', 
         'view' 
   );
   $result = array ();
   foreach ($options as $option) {
      if (isset($_GET[$option])) {
         $result[$option] = $_GET[$option];
      }
   }
   return $result;
}

function _filebrowser_to_utf8(&$node, $source) {
   return strcasecmp($node->folder_presentation->encoding, 'UTF-8') == 0 ? $source : mb_convert_encoding($source, "UTF-8", $node->folder_presentation->encoding);
}

function _filebrowser_to_fs(&$node, $source) {
   return strcasecmp($node->folder_presentation->encoding, 'UTF-8') == 0 ? $source : mb_convert_encoding($source, $node->folder_presentation->encoding, "UTF-8");
}

/**
 * @param result
 */
function _filebrowser_load_file_content($fid) {
   $content = db_fetch_array(db_query("SELECT * from {node_dir_listing_content} where fid=%d", $fid));
   return $content;
}

/**
 * Load data from current path.
 * @param source node
 */
function _filebrowser_load_files(& $node, $fid=null) {
   global $user, $base_url;
   
   // retreive paths options
   $options = _filebrowser_query_to_options($node);

   // convert any fid parameter to relative_path
   $fid = isset($options['fid']) ? $options['fid'] : $fid;
   if ($fid) {
      $content = _filebrowser_load_file_content($fid);
      if (!$content) {
         drupal_goto("node/$node->nid");
      }
      $relative_path = $content['path'];
   } else {
      $relative_path = '/';
   }
   $is_subdir = $relative_path != '/';
   
   // If we shouldn't be in a subdirectory, redirect to root_dir.
   if ($is_subdir && !_filebrowser_can_explore_subfolders($node)) {
      drupal_set_message(t('You\'re not allowed to browse sub folders.'));
      return;
   }
   
   // More advanced check to make sure no parent directories match our files.
   // blacklist
   if (!empty($relative_path)) {
      $dirs = explode('/', $relative_path);
      foreach ($dirs as $dir) {
         if (!empty($dir)) {
            if (_filebrowser_is_forbidden($node, $dir)) {
               drupal_set_message(t("You're not allowed to view '%dir'.", array (
                     '%dir' => $dir 
               )));
               return;
            }
         }
      }
   }
   // If this folder has already been processed, take it result
   static $cache = array ();
   if (isset($cache[$relative_path])) {
      return $node->file_listing = $cache[$relative_path];
   }
   
   // Load database folder content 
   $db_content = array ();
   $cursor = db_query("SELECT * FROM {node_dir_listing_content} where nid=%d and root = '%s'", $node->nid, $relative_path);
   while ($data = db_fetch_array($cursor)) {
      $db_content[$data['path']] = $data;
   }
   
   // Build full path
   $fs_root = realpath(_filebrowser_to_fs($node, _filebrowser_get_node_file_path($node)));
   $fs_root = realpath($fs_root . "/" . _filebrowser_to_fs($node, $relative_path)) . "/";
//   print("Root Full: " . _filebrowser_to_utf8($node, $fs_root) . "<br>");
   
   // Load meta-data
   $file_metadata = array ();
   // Check for meta files if we need info.
   $metadata_file = $fs_root . 'descript.ion';
   if (!file_exists($metadata_file)) {
      $metadata_file = $fs_root . 'file_name.bbs';
      if (file_exists($metadata_file)) {
         $file_metadata = "";
      }
   }
   if ($file_metadata != "") {
      $file_metadata = filebrowser_get_fileinfo($metadata_file);
   }
   
   // Iterate over files
   $files = array ();
   $files_count = 0;
   $total_size = 0;
   $folder_count = 0;
   $has_metadata = false;
   if (is_dir($fs_root) && $handler = opendir($fs_root)) {
      while (($fs_filename = readdir($handler)) !== false) {
         $fs_file_full_path = $fs_root . $fs_filename;
         
         // Check FS reading rights
         if (!is_readable($fs_file_full_path)) {
            continue;
         }
         
         // Check subfolder rights
         if (is_dir($fs_file_full_path) && !_filebrowser_can_explore_subfolders($node)) {
            continue;
         }
         
         // First file filtering
         if ($fs_filename == '.' || $fs_filename == '..') {
            // We are in the root folder, so we don't need an "up" entry
            if ($fs_filename == '..' && !$is_subdir) {
               continue;
            }
         
         } else {
            // This file is not filtered
            if (is_file($fs_file_full_path) && !_filebrowser_is_filtered($node, $fs_filename)) {
               continue;
            }
            
            // This file is not allowed
            if (_filebrowser_is_forbidden($node, $fs_filename)) {
               continue;
            }
         }
         
         // Build file relative path
         $filename = _filebrowser_to_utf8($node, $fs_filename);
         if ($filename == '.') {
            $file_relative_path = $relative_path;
         } elseif ($filename == '..') {
            $file_relative_path = _filebrowser_safe_dirname($relative_path);
            $content = db_fetch_array(db_query("SELECT * from {node_dir_listing_content} WHERE nid=%d and path='%s'", $node->nid, $file_relative_path));
            if ($content) {
               $db_content[$file_relative_path] = &$content;
            }
         
         } else {
            $file_relative_path = $relative_path . ($relative_path != '/' ? '/' : '') . $filename;
         }
//         print $file_relative_path . "<br>";
         
         // Build database file record
         if (!isset($db_content[$file_relative_path])) {
            $db_content[$file_relative_path] = array (
                  'exists' => true, 
                  'nid' => $node->nid, 
                  'root' => $relative_path, 
            			'path' => $file_relative_path 
            );
         }
         $db_content[$file_relative_path]['exists'] = true;
         $db_content[$file_relative_path]['display-name'] = $filename;
         
         $files[$filename] = array (
               'display-name' => $filename, 
               'full-path' => $file_relative_path, 
               'status' => MARK_READ, 
               'created' => 0, 
               'modified' => 0, 
               'size' => 0, 
               'kind' => is_file($fs_file_full_path) ? 0 : 1, 
               'mime-type' => !is_file($fs_file_full_path) ? "folder" : file_get_mimetype($fs_file_full_path), 
               'description' => isset($file_metadata[$fs_filename]) ? $file_metadata[$fs_filename] : '' 
         );
         
         $has_metadata |= !empty($file_metadata[$fs_filename]);
         
         // set file properties from stat()
         if (($f_stats = stat($fs_file_full_path)) !== FALSE) {
            if (is_file($fs_file_full_path)) {
               $files[$filename]['size'] = $f_stats['size'];
               $total_size += $files[$filename]['size'];
            
            }
            $files[$filename]['created'] = $f_stats['ctime'];
            $files[$filename]['modified'] = $f_stats['mtime'];
            if ($user->uid) {
               if ($user->access < $files[$filename]['created']) {
                  $files[$filename]['status'] = MARK_NEW;
               } else if ($user->access < $files[$filename]['modified']) {
                  $files[$filename]['status'] = MARK_UPDATED;
               }
            }
         }
         switch($files[$filename]['kind']) {
            case 0 :
               $files_count++;
               $total_size += $files[$filename]['size'];
               break;
            case 1 :
               if ($fs_filename != '.') {
                  $folder_count++;
               }
               break;
         }
         
         if ($fs_filename == '..') {
            $files[$filename]['mime-type'] .= "/parent";
            $files[$filename]['kind'] = 2;
         }
         
         $file_path = array ();
         if ($fs_filename == '..') {
            $parent_folder = dirname($relative_path);
            if ($parent_folder != "/") {
               $file_path['path'] = $parent_folder;
            }
         } else {
            $file_path['path'] = $relative_path . $fs_filename;
         }
      
      }
      
      // Set global folder properties
      $files['.']['size'] = $total_size;
      $files['.']['files_count'] = $files_count;
      $files['.']['folders_count'] = $folder_count;
      $files['.']['has-metadata'] = $has_metadata;
      $files['.']['relative-path'] = $relative_path;
      closedir($handler);
   }
   
   // Synchronize what we seen on filesystem with what is stored in database
   // We also build an access URL for each file as it is why we stored this stuff
   // in DB (have a unique ID for each file and path) to get rid of nationnal character
   // mess in URLS.
   $to_delete = array ();
   foreach ($db_content as $path => &$record) {
      if (!isset($record['exists']) 
      /*&& !file_exists(_filebrowser_to_fs($node, _filebrowser_get_node_file_path($node).$record['exists']))*/) {
         $to_delete[] = $record['fid'];
      } else {
         if (!isset($record['fid'])) {
            drupal_write_record('node_dir_listing_content', $record);
         }
         
         $key = $record['display-name'];
         $files[$key]['fid'] = $record['fid'];
         if ($files[$key]['kind'] != 0) {
            $files[$key]['url'] = url('node/' . $node->nid, array (
                  'query' => _filebrowser_options_to_query($options, array (
                        'fid' => $record['fid'] 
                  )), 
                  'absolute' => true 
            ));
         } else {
            $files[$key]['url'] = url('filebrowser/download/' . $record['fid'], array (
                  'absolute' => true 
            ));
         }
      }
   }
   
   // A quick way to drip obsolete records (FIXME not quite sure there is no limit in the number of 
   // items in the list
   if (count($to_delete)) {
      db_query("DELETE FROM {node_dir_listing_content} where fid in (" . implode(",", $to_delete) . ")");
   }
   
   // Cache update
   // FIXME : what of two nodes point to the same folder ? 
   $cache[$relative_path] = & $files;
   
   // Add all this data to the node
   $node->file_listing = $files;
}

/**
 * Helper function to match a pattern on the path
 * @param path path to process
 * @param patterns to search (seperated by cr)
 * @return true if at least one pattern is found
 */
function _filebrowser_match_path($path, $patterns) {
   static $regexps = null;
   
   if (!isset($regexps[$patterns])) {
      $regexps[$patterns] = '/^(' . preg_replace(array (
            '/(\r\n?|\n)/', 
            '/\\\\\*/' 
      ), array (
            '|', 
            '.*' 
      ), preg_quote($patterns, '/')) . ')$/';
   }
   $result = preg_match($regexps[$patterns], $path) == 1;
   return $result;
}

function _filebrowser_filter_checkboxes_result($array) {
   $result = array ();
   if ($array) {
      foreach ($array as $key => $value) {
         if (!empty($value)) {
            $result[$key] = true;
         }
      }
   }
   return $result;
}

function _filebrowser_properties_to_checkboxes(& $properties) {
   $result = array ();
   if ($properties) {
      foreach ($properties as $key => $value) {
         if ($value) {
            $result[$key] = $key;
         }
      }
   }
   return $result;
}

/**
 * Theming function for icon-view.
 *
 * @param $node the node to transform to icon  view.
 * @return A string containing icon view XHTML.
 */
function theme_dir_listing_icon_view($node) {
   $files = & $node->file_listing;
   
   // Record if directory listing is empty or not.
   $directory_empty = TRUE;
   $output = "<div class='dir-listing-icon-view'>";
   // Considered each file (or directory)
   foreach ($files as $file_name => $data) {
      // Skip dot folder
      if ($file_name == ".") {
         continue;
      }
      
      // At least one file
      $directory_empty = FALSE;
      
      // File extension case
      if ($file_name != ".." && $data['kind'] == 0 && $node->folder_presentation->hide_extension) {
         $pos = strrpos($data['name'], ".");
         $data[FILEBROWSER_DATA_NAME_DISPLAY_NAME] = substr($data['name'], 0, $pos);
      }
      $thumbnail = filebrowser_create_thumbnail($node, $data);
      $href = $data['url'];
      $title = $data['display-name'] == '..' ? t('Go up') : $data['display-name'];
      $new = theme('mark', $data['status']);
      $description = $data['description'];
      $output .= theme('dir_listing_icon', $data, $thumbnail, $title, $description, $href, $new);
   }
   $output .= "</div>";
   return $output;
}

function theme_dir_listing_statistics($statistics) {
   $output = "<div class='dir-listing-status'>";
   if ($statistics['empty']) {
      $output .= $statistics['empty'];
   } else {
      $output .= implode(" - ", $statistics);
   }
   $output .= "</div>";
   return $output;
}

/**
 * Theming function for list view.
 *
 * @param $node the node to transform to list view.
 * @return A string containing list view XHTML.
 */
function theme_dir_listing_list_view($node) {
   $files = & $node->file_listing;
   // Deal with files.
   $unsorted_rows = array ();
   
   // add optional colunmns
   $node->folder_presentation->visible_columns['description'] = true;
   
   // Visible columns
   $visible_columns = array ();
   $column_names = _filebrowser_columns();
   
   // Record if directory listing is empty or not.
   $directory_empty = TRUE;
   
   // Considered each file (or directory)
   foreach ($files as $file_name => $data) {
      // Skip dot folder
      if ($file_name == ".") {
         continue;
      }
      
      // At least one file
      $directory_empty = FALSE;
      
      // File extension case
      if ($file_name != ".." && $data['kind'] == 0 && $node->folder_presentation->hide_extension) {
         $pos = strrpos($data['name'], ".");
         $data[FILEBROWSER_DATA_NAME_DISPLAY_NAME] = substr($data['name'], 0, $pos);
      }
      
      foreach ($column_names as $column_name => $column_title) {
         if (isset($node->folder_presentation->visible_columns[$column_name]) && $node->folder_presentation->visible_columns[$column_name]) {
            switch($column_name) {
               case FILEBROWSER_DATA_NAME_ICON :
                  $visible_columns[$column_name] = true;
                  $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_ICON] = array (
                        'data' => filebrowser_create_thumbnail($node, $data), 
                        'style' => 'width:1%;' 
                  );
                  break;
               
               case FILEBROWSER_DATA_NAME_DISPLAY_NAME :
                  $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_DISPLAY_NAME] = '<a href="' . $data['url'] . '">' . ($data['display-name'] == '..' ? t('Go up') : $data['display-name']) . '</a>' . theme('mark', $data['status']);
                  $visible_columns[$column_name] = true;
                  break;
               
               case FILEBROWSER_DATA_NAME_SIZE :
                  $used = $data['size'] != 0;
                  $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_SIZE] = $used ? format_size($data['size']) : "0";
                  if ($used) {
                     $visible_columns[$column_name] = true;
                  }
                  break;
               
               case FILEBROWSER_DATA_NAME_CREATED :
                  if ($data['created'] != 0) {
                     $visible_columns[$column_name] = true;
                  }
                  $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_CREATED] = format_date($data['created'], "small");
                  break;
               
               case FILEBROWSER_DATA_NAME_MODIFIED :
                  if ($data['modified'] != 0) {
                     $visible_columns[$column_name] = true;
                  }
                  $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_MODIFIED] = format_date($data['modified'], "small");
                  break;
               
               case FILEBROWSER_DATA_NAME_TYPE :
                  $used = $data['mime-type'] != 'folder' && $data['mime-type'] != 'folder/parent';
                  if ($used) {
                     $visible_columns[$column_name] = true;
                  }
                  $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_TYPE] = $used ? t($data['mime-type']) : "";
                  break;
               
               case FILEBROWSER_DATA_NAME_DESCRIPTION :
                  if (!empty($data['description'])) {
                     $visible_columns[$column_name] = true;
                  }
                  $unsorted_rows[$file_name]['description'] = $data['description'];
                  break;
               
               default :
                  // TODO : defensive programming
                  // Define an action when we arrived here.
                  break;
            }
         }
      }
   }
   
   if (!$directory_empty) {
      // Builder header and clean up unused columns
      $header = array ();
      foreach ($column_names as $column_name => $column_title) {
         if (isset($visible_columns[$column_name]) && $visible_columns[$column_name]) {
            if ($column_name == FILEBROWSER_DATA_NAME_ICON) {
               $header[$column_name] = array ();
            } else {
               $header[$column_name] = array (
                     'data' => $column_title, 
                     'field' => $column_name 
               );
            }
         } else {
            // If unused data then clean up !
            foreach ($unsorted_rows as & $row) {
               unset($row[$column_name]);
            }
         }
      }
      $header[$node->folder_presentation->default_sort]['sort'] = $node->folder_presentation->default_sort_order;
      
      // Handle any and all sorting.
      $table_sort = tablesort_init($header);
      
      // Split files in two heaps to preserve folders and files
      $just_folders = array ();
      $just_files = array ();
      
      // TODO : take data from $unsorted_rows and not $data
      foreach ($files as $data) {
         if ($data['kind'] == 0) {
            $just_files[] = $data;
         } else {
            // Do not retain the '.' folder
            if ($data['display-name'] != '.') {
               $just_folders[] = $data;
            }
         }
      }
      
      // Sort files according to correct column.
      if ($table_sort['sql']) {
         usort($just_folders, "_filebrowser_sorter_" . $table_sort['sql']);
         if ($table_sort['sort'] == 'asc') {
            $just_folders = array_reverse($just_folders, TRUE);
         }
         usort($just_files, "_filebrowser_sorter_" . $table_sort['sql']);
         if ($table_sort['sort'] == 'asc') {
            $just_files = array_reverse($just_files, TRUE);
         }
      }
      // Combine folders and files array and setup correct order.
      //$rows = array_merge($just_folders, $just_files);
      $rows = array ();
      foreach ($just_folders as $data) {
         $rows[] = $unsorted_rows[$data['display-name']];
      }
      foreach ($just_files as $data) {
         $rows[] = $unsorted_rows[$data['display-name']];
      }
      $output .= theme('table', $header, $rows);
      return $output;
   } // Directory is not empty


}

/**
 * Loads file metainformation from the specified file. Also
 * allows the file to specify a *callback* with which the
 * descriptions are parsed, so more metainformation can be
 * presented on the output.
 */
function filebrowser_get_fileinfo($info_file_path) {
   static $metacols = array ();
   
   // Build meta information list.
   $metainfo = array ();
   if (is_readable($info_file_path) && ($file = file($info_file_path))) {
      foreach ($file as $line) {
         // Skip empty and commented lines.
         if (trim($line) == '' || strpos(trim($line), '#') === 0) {
            continue;
         }
         
         // Use PCRE regular expressions to parse file.
         $matches = array ();
         preg_match('/"([^"]+)"\s+(.*)|(\S+)\s+(.*)/', $line, $matches);
         $name = !empty($matches[1]) ? $matches[1] : $matches[3];
         $description = !empty($matches[2]) ? $matches[2] : $matches[4];
         
         if (isset($metainfo[$name])) {
            $metainfo[$name] .= ' ' . trim($description);
         } else {
            $metainfo[$name] = trim($description);
         }
      }
   }
   return $metainfo;
}

/**
 * Create a thumbnail and the associated XHTML code for a specific file.
 * @param $node source node
 * @param $file source file
 * @return XHTML code
 */
function filebrowser_create_thumbnail(& $node, & $file) {
   static $thumbnailers = null;
   if (!$thumbnailers) {
      $thumbnailers = module_implements("filebrowser_thumbnailer");
   }
   if (count($thumbnailers) != 0) {
      foreach ($thumbnailers as $thumbnailer) {
         if ($node->$thumbnailer->enabled) {
            $thumbnail = module_invoke($thumbnailer, "filebrowser_thumbnailer", $file, $node->$thumbnailer);
            if ($thumbnail) {
               return $thumbnail;
            }
         }
      }
   }
   return _filebrowser_get_default_thumbnail($file);
}

function _filebrowser_can_download_archive(& $node) {
   return (node_access('view', $node) && $node->folder_rights->download_archive && user_access(FILEBROWSER_DOWNLOAD_ARCHIVE));
}

function _filebrowser_can_download_file(& $node) {
   return (node_access('view', $node) && user_access(FILEBROWSER_DOWNLOAD));
}

function _filebrowser_is_filtered(& $node, $file) {
   return trim($node->folder_rights->filtered_files) == '' || _filebrowser_match_path($file, $node->folder_rights->filtered_files);
}

function _filebrowser_is_forbidden(& $node, $file) {
   return trim($node->folder_rights->forbidden_files) != '' && _filebrowser_match_path($file, $node->folder_rights->forbidden_files);
}

function _filebrowser_can_explore_subfolders(& $node) {
   return $node->folder_rights->explore_subdirs;
}

function _filebrowser_thumbnail_cache_name($src, $size) {
   $thumbnail_path = file_directory_path() . "/filebrowser";
   
   if (!file_exists($thumbnail_path)) {
      mkdir($thumbnail_path);
   }
   $thumbnail_path .= "/" . str_replace("/", "_", $size . "-" . $src) . ".jpg";
   return $thumbnail_path;
}

function _filebrowser_safe_basename($path) {
   $path = rtrim($path, '/');
   $path = explode('/', $path);
   return end($path);
}

function _filebrowser_safe_dirname($path) {
   $path = rtrim($path, '/');
   $path = explode('/', $path);
   array_pop($path);
   $result = implode("/", $path);
   if ($result == '') {
      return '/';
   }
   return $result;
}

/**
 * 
 */
function _filebrowser_node_content_cleanup($node) {
   db_query('DELETE FROM {node_dir_listing_content} WHERE nid = %d', $node->nid);
}