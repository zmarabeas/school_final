#!/bin/bash

# Test script for NPR All Things Considered recording system
# This script performs various tests to verify the system works

SCRIPT_DIR="/home/catred/penguin/School/fall2025/os/lecture/assignments/01/task3"
RECORD_SCRIPT="$SCRIPT_DIR/record_npr.sh"
RECORDINGS_DIR="$SCRIPT_DIR/recordings"
LOGS_DIR="$SCRIPT_DIR/logs"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print test results
print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓ PASS${NC}: $2"
    else
        echo -e "${RED}✗ FAIL${NC}: $2"
    fi
}

echo "=== NPR Recording System Test Suite ==="
echo "Testing system components and functionality..."
echo ""

# Test 1: Check if directories exist
echo "1. Testing directory structure..."
test -d "$SCRIPT_DIR" && test -d "$RECORDINGS_DIR" && test -d "$LOGS_DIR"
print_result $? "Directory structure exists"

# Test 2: Check if main script exists and is executable
echo "2. Testing main recording script..."
test -f "$RECORD_SCRIPT" && test -x "$RECORD_SCRIPT"
print_result $? "record_npr.sh exists and is executable"

# Test 3: Test stream connectivity
echo "3. Testing stream connectivity..."
timeout 10 curl -s -I "https://playerservices.streamtheworld.com/api/livestream-redirect/WUOMFM.mp3" > /dev/null 2>&1
result1=$?
timeout 10 curl -s -I "https://26233.live.streamtheworld.com:443/WUOMFM.mp3" > /dev/null 2>&1
result2=$?

if [ $result1 -eq 0 ] || [ $result2 -eq 0 ]; then
    print_result 0 "At least one stream URL is accessible"
else
    print_result 1 "No stream URLs are accessible"
fi

# Test 4: Test short recording (30 seconds)
echo "4. Testing short recording (30 seconds)..."
TEST_FILE="$RECORDINGS_DIR/test_$(date +%Y%m%d_%H%M%S).mp3"

timeout 35 curl -s -L -o "$TEST_FILE" \
    "https://playerservices.streamtheworld.com/api/livestream-redirect/WUOMFM.mp3" > /dev/null 2>&1 &
CURL_PID=$!

# Wait 30 seconds then kill the process
sleep 30
kill $CURL_PID 2>/dev/null
wait $CURL_PID 2>/dev/null

# Check if test file was created and has content
if [ -f "$TEST_FILE" ] && [ -s "$TEST_FILE" ]; then
    FILE_SIZE=$(stat -c%s "$TEST_FILE" 2>/dev/null || echo 0)
    if [ $FILE_SIZE -gt 10000 ]; then  # At least 10KB for 30 seconds
        print_result 0 "Short recording test successful (${FILE_SIZE} bytes)"
        rm "$TEST_FILE"  # Clean up test file
    else
        print_result 1 "Test file too small (${FILE_SIZE} bytes)"
    fi
else
    print_result 1 "Short recording test failed - no file created"
fi

# Test 5: Check cron job syntax
echo "5. Testing cron job setup script..."
test -f "$SCRIPT_DIR/setup_cron.sh" && test -x "$SCRIPT_DIR/setup_cron.sh"
print_result $? "setup_cron.sh exists and is executable"

# Test 6: Verify required tools are available
echo "6. Testing required system tools..."
TOOLS_AVAILABLE=0
for tool in curl timeout crontab date stat mkdir chmod; do
    if command -v $tool >/dev/null 2>&1; then
        echo -e "  ${GREEN}✓${NC} $tool available"
    else
        echo -e "  ${RED}✗${NC} $tool missing"
        TOOLS_AVAILABLE=1
    fi
done
print_result $((1 - TOOLS_AVAILABLE)) "All required tools are available"

# Test 7: Test log file creation
echo "7. Testing log file creation..."
TEST_LOG="$LOGS_DIR/test_$(date +%Y%m%d_%H%M%S).log"
echo "Test log entry" > "$TEST_LOG"
if [ -f "$TEST_LOG" ] && [ -s "$TEST_LOG" ]; then
    print_result 0 "Log file creation successful"
    rm "$TEST_LOG"  # Clean up
else
    print_result 1 "Log file creation failed"
fi

# Test 8: Check disk space
echo "8. Testing available disk space..."
AVAILABLE_SPACE=$(df "$RECORDINGS_DIR" | tail -1 | awk '{print $4}')
# Need at least 1GB (1000000 KB) for recordings
if [ $AVAILABLE_SPACE -gt 1000000 ]; then
    print_result 0 "Sufficient disk space available ($(($AVAILABLE_SPACE/1024))MB free)"
else
    print_result 1 "Insufficient disk space ($(($AVAILABLE_SPACE/1024))MB free, need >1GB)"
fi

echo ""
echo "=== Test Summary ==="
echo "Review any failed tests above and fix issues before running the recording system."
echo ""
echo "To set up the cron job, run:"
echo "  $SCRIPT_DIR/setup_cron.sh"
echo ""
echo "To manually test a full recording, run:"
echo "  $RECORD_SCRIPT"
echo ""
echo "Recordings will be saved to: $RECORDINGS_DIR"
echo "Logs will be saved to: $LOGS_DIR"