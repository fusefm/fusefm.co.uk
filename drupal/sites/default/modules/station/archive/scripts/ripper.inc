; $Id: ripper.inc,v 1.6 2009/08/29 08:01:42 drewish Exp $
; These are the settings for the stream ripper script.

; Complete URL of webstream to rip
stream_url = "http://127.0.0.1:8000/high"

; Full path to the stream ripper executable, version 1.61.17 or higher.
streamripper_path = "/usr/local/bin/streamripper"

; Full path to station archive module's import directory (no trailing slash)
import_path = "/usr/local/www/drupal/sites/all/modules/station/archive/import"

; Because shows tend to start early or finish late, the overlap_seconds
; setting allows you to determine how much the recording should overlap at the
; end of the hour. The suggestion is to schedule the script to start one minute
; before the hour and use 60 seconds of overlap. That will result in a 62 minute
; recording, with one minute before the hour and an extra minute at the end.
overlap_seconds = 60

; The file type of the stream; "mp3" and "ogg" are supported.
file_format = "mp3"
