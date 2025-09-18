#!/bin/bash

SCRIPT_PATH="$(dirname "$0")/record_npr.sh"

mkdir -p "$(dirname "$0")/recordings" "$(dirname "$0")/logs"

# add daily 4 PM cron job     min h d m dayOfWeek
(crontab -l 2>/dev/null; echo "0 16 * * * $SCRIPT_PATH") | crontab -

echo "Cron job set for daily 4 PM NPR recording"
