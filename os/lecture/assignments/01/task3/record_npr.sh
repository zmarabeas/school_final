#!/bin/bash

# NPR All Things Considered Recording Script
# Records 2 hours of Michigan Public Radio (WUOM 91.7)
# Schedule: Weekdays 3:00-5:00 PM Central (4:00-6:00 PM Eastern)

# Configuration
RECORDINGS_DIR="/home/catred/penguin/School/fall2025/os/lecture/assignments/01/task3/recordings"
LOGS_DIR="/home/catred/penguin/School/fall2025/os/lecture/assignments/01/task3/logs"
DURATION=7200  # 2 hours in seconds
DATE_FORMAT=$(date '+%Y-%m-%d_%H%M')
FILENAME="ATC_${DATE_FORMAT}.mp3"
LOG_FILE="${LOGS_DIR}/npr_recording_${DATE_FORMAT}.log"

# Stream URLs (primary uses redirect service, backup is direct)
PRIMARY_STREAM="https://playerservices.streamtheworld.com/api/livestream-redirect/WUOMFM.mp3"
BACKUP_STREAM="https://26233.live.streamtheworld.com:443/WUOMFM.mp3"

# Function to log messages
log_message() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1" | tee -a "$LOG_FILE"
}

# Function to record stream
record_stream() {
    local stream_url="$1"
    local attempt="$2"
    
    log_message "Starting recording attempt #${attempt} from: $stream_url"
    log_message "Recording to: ${RECORDINGS_DIR}/${FILENAME}"
    log_message "Duration: ${DURATION} seconds (2 hours)"
    
    # Use timeout to limit recording to exactly 2 hours
    # Follow redirects (-L), output to file (-o), continue on errors (-C -)
    timeout $DURATION curl -L -C - -o "${RECORDINGS_DIR}/${FILENAME}" \
        -H "User-Agent: NPR-Recorder/1.0" \
        --connect-timeout 30 \
        --max-time $((DURATION + 60)) \
        "$stream_url" >> "$LOG_FILE" 2>&1
    
    local exit_code=$?
    
    if [ $exit_code -eq 0 ] || [ $exit_code -eq 124 ]; then
        # Exit code 0 = success, 124 = timeout (expected after 2 hours)
        log_message "Recording completed successfully"
        
        # Check if file was created and has reasonable size
        if [ -f "${RECORDINGS_DIR}/${FILENAME}" ]; then
            local file_size=$(stat -c%s "${RECORDINGS_DIR}/${FILENAME}")
            log_message "File size: ${file_size} bytes"
            
            # Minimum expected size for 2 hours at 64kbps: ~57MB
            if [ $file_size -gt 50000000 ]; then
                log_message "Recording appears successful - good file size"
                return 0
            else
                log_message "WARNING: File size seems too small for 2-hour recording"
                return 1
            fi
        else
            log_message "ERROR: Recording file was not created"
            return 1
        fi
    else
        log_message "ERROR: Recording failed with exit code $exit_code"
        return $exit_code
    fi
}

# Main execution
log_message "=== NPR All Things Considered Recording Started ==="
log_message "Date: $(date)"
log_message "Expected recording duration: 2 hours"

# Ensure directories exist
mkdir -p "$RECORDINGS_DIR" "$LOGS_DIR"

# Attempt recording with primary stream
if ! record_stream "$PRIMARY_STREAM" "1"; then
    log_message "Primary stream failed, trying backup stream"
    
    # Remove failed file if it exists
    [ -f "${RECORDINGS_DIR}/${FILENAME}" ] && rm "${RECORDINGS_DIR}/${FILENAME}"
    
    # Try backup stream
    if ! record_stream "$BACKUP_STREAM" "2"; then
        log_message "ERROR: Both stream attempts failed"
        log_message "=== Recording Failed ==="
        exit 1
    fi
fi

log_message "=== Recording Session Completed Successfully ==="

# Optional: Clean up old recordings (keep last 30 days)
find "$RECORDINGS_DIR" -name "ATC_*.mp3" -mtime +30 -delete 2>/dev/null

exit 0