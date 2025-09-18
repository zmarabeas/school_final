#!/bin/bash

RECORDINGS_DIR="$(dirname "$0")/recordings"
LOGS_DIR="$(dirname "$0")/logs"
FILENAME="npr_atc_$(date +%Y-%m-%d).mp3"
STREAM_URL="https://playerservices.streamtheworld.com/api/livestream-redirect/WUOMFM.mp3"

mkdir -p "$RECORDINGS_DIR" "$LOGS_DIR"

echo "$(date): Starting NPR recording - $FILENAME" >> "$LOGS_DIR/recording.log"

# record for 2 hours (7200 seconds)
timeout 7200 wget -O "$RECORDINGS_DIR/$FILENAME" "$STREAM_URL"

echo "$(date): Recording completed - $FILENAME" >> "$LOGS_DIR/recording.log"
