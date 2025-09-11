#!/bin/bash

# Setup script for NPR All Things Considered cron job
# This script adds the recording job to the user's crontab

SCRIPT_PATH="/home/catred/penguin/School/fall2025/os/lecture/assignments/01/task3/record_npr.sh"
CRON_ENTRY="0 15 * * 1-5 $SCRIPT_PATH"

echo "Setting up NPR All Things Considered recording cron job..."
echo "Schedule: Weekdays at 3:00 PM Central Time (4:00 PM Eastern)"
echo "Script: $SCRIPT_PATH"
echo ""

# Check if script exists and is executable
if [ ! -f "$SCRIPT_PATH" ]; then
    echo "ERROR: Recording script not found at $SCRIPT_PATH"
    exit 1
fi

if [ ! -x "$SCRIPT_PATH" ]; then
    echo "ERROR: Recording script is not executable"
    echo "Run: chmod +x $SCRIPT_PATH"
    exit 1
fi

# Check if cron job already exists
if crontab -l 2>/dev/null | grep -q "$SCRIPT_PATH"; then
    echo "Cron job already exists. Current crontab entries for this script:"
    crontab -l | grep "$SCRIPT_PATH"
    echo ""
    echo "Remove existing entries? (y/N): "
    read -r response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        # Remove existing entries
        crontab -l 2>/dev/null | grep -v "$SCRIPT_PATH" | crontab -
        echo "Existing entries removed."
    else
        echo "Keeping existing entries. Exiting without changes."
        exit 0
    fi
fi

# Add new cron job
echo "Adding cron job: $CRON_ENTRY"
(crontab -l 2>/dev/null; echo "$CRON_ENTRY") | crontab -

if [ $? -eq 0 ]; then
    echo "✓ Cron job added successfully!"
    echo ""
    echo "Current crontab:"
    crontab -l | grep -E "(record_npr|ATC|All Things)"
    echo ""
    echo "The system will now automatically record NPR's All Things Considered"
    echo "every weekday from 3:00-5:00 PM Central Time."
    echo ""
    echo "Recordings will be saved to:"
    echo "  /home/catred/penguin/School/fall2025/os/lecture/assignments/01/task3/recordings/"
    echo ""
    echo "Logs will be saved to:"
    echo "  /home/catred/penguin/School/fall2025/os/lecture/assignments/01/task3/logs/"
else
    echo "ERROR: Failed to add cron job"
    exit 1
fi