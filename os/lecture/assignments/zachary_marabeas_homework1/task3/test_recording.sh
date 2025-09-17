#!/bin/bash

STREAM_URL="https://playerservices.streamtheworld.com/api/livestream-redirect/WUOMFM.mp3"

echo "Testing NPR stream connectivity..."

# test if stream is accessable
if timeout 10 wget --spider "$STREAM_URL" 2>/dev/null; then
    echo "Stream is accessible"
else
    echo "Stream connection failed"
    exit 1
fi

echo "Testing 10-second recording..."

# test recording
timeout 10 wget -O "test_recording.mp3" "$STREAM_URL" 2>/dev/null

if [ -f "test_recording.mp3" ] && [ -s "test_recording.mp3" ]; then
    echo "Recording test successful"
    rm "test_recording.mp3"
else
    echo "Recording test failed"
    exit 1
fi

echo "All tests passed!"
