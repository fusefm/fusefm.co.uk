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

function filebrowser_callback_create_thumbnails($node) {
   static $thumbnailers=NULL;
   if (!$thumbnailers) {
      $thumbnailers = module_implements("filebrowser_thumbnailer");
   }
   $jobs = array ();
   if (count($thumbnailers) != 0) {
      foreach ($thumbnailers as $thumbnailer) {
         if ($node->$thumbnailer->enabled) {
            $result = module_invoke($thumbnailer, "filebrowser_prepare_thumbnailer", $node->file_listing, $node->$thumbnailer);
            if ($result) {
               $jobs = array_merge($jobs, $result);
            }
         }
      }
   }
   $operations = array ();
   foreach ($jobs as $job) {
      $operations[] = array (
            '_filebrowser_create_thumbnails', 
            $job 
      );
   }
   if (count($operations) > 0) {
      $batch = array (
            'title' => t('Creating thumbnails'), 
            'operations' => $operations, 
            'file' => drupal_get_path("module", "filebrowser") . "/filebrowser.pages.inc" 
      );
      batch_set($batch);
      batch_process("node/" . $node->nid);
   } else {
      return t("All thumbnails are up to date");
   }
}

function _filebrowser_create_thumbnails($callback, $arguments, &$context) {
   if (!isset($context['sandbox']['progress'])) {
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['max'] = 1;
   }
   call_user_func_array($callback, $arguments);
   $context['sandbox']['progress']++;
   //	$context['message']=t('processing @file (@res size)', array('@file'=>$file[0], '@size'=>$file[2]));
   //	if ($context['sandbox']['progress']!=$context['sandbox']['max']) {
   $context['finished'] = 1; //$context['sandbox']['progress'] / $context['sandbox']['max'];
//	}
}

/**
 * Callback for filebrowser_download/%node menu.
 */
function filebrowser_callback_download($fid) {
   $content = _filebrowser_load_file_content($fid);
   $node = null;
   $target = null;
   if ($content) {
      $node = node_load($content['nid']);
      $target = _filebrowser_to_fs($node, _filebrowser_get_node_file_path($node) . $content['path']);
   }
   if (!$content || !$node || !_filebrowser_can_download_file($node) || !$target) {
      drupal_access_denied();
      exit();
   }

   if (is_dir($target)) {
      if (!function_exists("zip_open")) {
         drupal_set_message(t("No ZIP support found in PHP installation, please contact your administrator"));
         return;
      }
      $zip = new ZipArchive();
      $target = file_directory_path() . "/filebrowser/archives/" . _filebrowser_safe_basename($target) . ".zip";
      mkdir(_filebrowser_safe_dirname($target), 0777,true);
      if ($target) {
         unlink($target);
      }
      _filebrowser_load_files($node, $fid);
  
      if ($zip->open($target, ZIPARCHIVE::CREATE) === TRUE) {
         foreach ($node->file_listing as $file_name => $file_data) {
                   var_dump($file_data);
            if ($file_data['kind'] === 0) {
               $fs_filename = realpath(_filebrowser_to_fs($node, _filebrowser_get_node_file_path($node) . "/" . $file_data['full-path']));
                   var_dump($fs_filename);
                   $zip->addFile($fs_filename, $file_name);
            }
         }
         $zip->close();
      } else {
         return t("Unable to create temporary zip file '@file'", array (
               file => $target 
         ));
      }
   }

   $decoded_file = _filebrowser_to_utf8($node, $target);
   header('Content-Description: File Transfer');
   header("Cache-Control: public, must-revalidate, max-age=0"); // HTTP/1.1
   header("Pragma: public");
   header("Expires: Sat, 26 Jul 1997 05:00:00 GMT"); // Date in the past
   header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
   header("Content-Type: " . file_get_mimetype($decoded_file));
   header("Content-Transfer-Encoding: binary");
   header("Content-Length: " . filesize($target));
   header('Content-Disposition: attachment; filename="' . _filebrowser_safe_basename($decoded_file) . '";');
   switch($node->folder_rights->download_manager) {
      case 0 :
         header("Location: " . url(trim($target, '/'), array (
               'absolute' => true 
         )));
         break;
      case 1 :
         $block_size = 4096;
         $buffer = '';
         $handle = fopen($target, 'rb');
         if ($handle === false) {
            return false;
         }
         while (!feof($handle)) {
            $buffer = fgets($handle, $block_size);
            echo $buffer;
         }
         fclose($handle);
         break;
      case 2 :
         $randomstring = md5(rand() . time() . rand());
         $obscurity_link = file_directory_path() . "/filebrowser/obscurity_links/$randomstring/" . _filebrowser_safe_basename($target);
         mkdir(_filebrowser_safe_dirname($obscurity_link), 0777,true);
         symlink(realpath($target), $obscurity_link);
         header("Location: " . base_path().$obscurity_link);
         break;
   }
   exit();
}


