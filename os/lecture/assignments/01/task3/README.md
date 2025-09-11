# NPR All Things Considered Auto-Recording System

A Linux-based automated recording system for NPR's "All Things Considered" program from Michigan Public Radio using only native command-line tools.

## Overview

This system automatically records NPR's "All Things Considered" program every weekday from **3:00-5:00 PM Central Time** (4:00-6:00 PM Eastern Time) for exactly 2 hours using Michigan Public Radio's live stream.

## System Requirements

- Linux system with bash shell
- Native tools: `curl`, `timeout`, `crontab`, `date`, `stat`, `mkdir`, `chmod`
- Internet connection for stream access
- Minimum 1GB free disk space for recordings

## Key Technical Details

### Stream Configuration
- **Primary Stream URL**: `https://playerservices.streamtheworld.com/api/livestream-redirect/WUOMFM.mp3`
- **Backup Stream URL**: `https://26233.live.streamtheworld.com:443/WUOMFM.mp3`
- **Stream Format**: MP3, 64kbps (audio/mpeg)
- **Station**: WUOM-FM (Michigan Public Radio 91.7 FM Ann Arbor)

### Recording Details
- **Duration**: Exactly 2 hours (7200 seconds) using `timeout`
- **Schedule**: Weekdays 3:00 PM Central (`0 15 * * 1-5` cron format)
- **File Naming**: `ATC_YYYY-MM-DD_HHMM.mp3`
- **Expected File Size**: ~57MB per 2-hour recording at 64kbps

### Directory Structure
```
task3/
├── record_npr.sh          # Main recording script
├── setup_cron.sh          # Cron job installation script
├── test_recording.sh      # System test and verification
├── recordings/            # Recorded MP3 files
├── logs/                  # Recording session logs
└── README.md             # This documentation
```

## Installation & Setup

### 1. Run System Tests
```bash
./test_recording.sh
```
This validates:
- Directory structure
- Script permissions  
- Stream connectivity
- Required tools availability
- Short recording test
- Disk space check

### 2. Install Cron Job
```bash
./setup_cron.sh
```
This adds the automated weekday recording schedule to your crontab.

### 3. Verify Installation
```bash
crontab -l | grep record_npr
```
Should show: `0 15 * * 1-5 /path/to/record_npr.sh`

## Manual Operation

### Test Recording
Run a full 2-hour recording manually:
```bash
./record_npr.sh
```

### Check Logs
View the latest recording log:
```bash
tail -f logs/npr_recording_*.log
```

### List Recordings
```bash
ls -la recordings/
```

## How It Works

### Recording Process
1. **Stream Selection**: Tries primary stream URL, falls back to backup if needed
2. **Duration Control**: Uses `timeout 7200` to limit recording to exactly 2 hours
3. **File Output**: Uses `curl -L -C -` with resume capability and redirect following
4. **Error Handling**: Logs all activity, validates file size, retries with backup stream
5. **Cleanup**: Automatically removes recordings older than 30 days

### Cron Automation
- **Schedule**: `0 15 * * 1-5` (3:00 PM Monday-Friday Central Time)
- **Logging**: Each recording session creates timestamped log files
- **Reliability**: Built-in stream fallback and error recovery

### Key Script Features
- **Dual Stream URLs**: Primary uses redirect service, backup is direct stream
- **Resume Support**: `curl -C -` allows resuming interrupted downloads  
- **User Agent**: Sets `NPR-Recorder/1.0` for proper stream identification
- **Timeout Safety**: Multiple timeout mechanisms prevent runaway processes
- **Size Validation**: Checks recorded file size to detect failed recordings

## Troubleshooting

### Common Issues

**No recordings created**:
- Check internet connection
- Verify stream URLs are accessible: `curl -I [stream-url]`
- Check disk space availability
- Review log files for errors

**Small file sizes**:
- Network interruptions during recording
- Stream connectivity issues
- Check logs for curl error messages

**Cron job not running**:
- Verify cron service is running: `systemctl status cron`
- Check crontab entry: `crontab -l`
- Ensure script has execute permissions: `ls -la record_npr.sh`

### Log Analysis
Each recording creates detailed logs with:
- Start/end timestamps
- Stream URLs attempted
- File sizes and validation results
- Error messages and exit codes
- Network timeout information

### Manual Testing
Run the test script to diagnose issues:
```bash
./test_recording.sh
```

## File Management

### Storage Requirements
- Each 2-hour recording: ~57MB (64kbps MP3)
- Weekly storage: ~285MB (5 recordings)
- Monthly storage: ~1.2GB (22 recordings average)

### Automatic Cleanup
The recording script automatically removes recordings older than 30 days to manage disk space.

### Manual Cleanup
```bash
# Remove recordings older than 7 days
find recordings/ -name "ATC_*.mp3" -mtime +7 -delete

# Remove old log files
find logs/ -name "*.log" -mtime +14 -delete
```

## Technical Implementation Notes

### Stream URL Discovery
- Michigan Public Radio uses StreamTheWorld CDN
- Primary URL uses redirect service for load balancing
- Backup URL points directly to stream server
- Both URLs tested during system development on 2025-09-10

### Error Recovery Strategy
1. Attempt primary stream URL
2. On failure, remove partial file and retry with backup URL
3. Validate final recording size (minimum 50MB for 2-hour recording)
4. Log all attempts and results

### Native Tool Advantages  
- **No dependencies**: Uses only standard Linux utilities
- **High reliability**: Native tools are well-tested and stable
- **Resource efficient**: Minimal system overhead
- **Universal compatibility**: Works on any Linux distribution

## Program Information

**All Things Considered** is NPR's flagship evening news program, featuring:
- Live broadcast: 4:00-6:30 PM Eastern (3:00-5:30 PM Central) 
- Core program: 4:00-6:00 PM Eastern (recorded by this system)
- Content: News, analysis, interviews, commentary, special features
- Re-feeds with updates until 10:00 PM ET

This recording system captures the main 2-hour program segment that contains the primary daily content.