# NPR All Things Considered Auto-Recording System

Minimal Linux-based automated recording system for NPR's "All Things Considered" program using only native command-line tools.

## Overview

Records NPR's "All Things Considered" program daily at **4:00 PM** for exactly 2 hours using Michigan Public Radio's live stream.

## Requirements

- Linux system with bash shell
- Native tools: `wget`, `timeout`, `crontab`, `date`, `mkdir`
- Internet connection

## Files

- `record_npr.sh` - Main recording script (12 lines)
- `setup_cron.sh` - One-time cron setup (8 lines)
- `test_recording.sh` - Stream connectivity test (17 lines)

## Setup

1. **Test connectivity:**
   ```bash
   ./test_recording.sh
   ```

2. **Install cron job:**
   ```bash
   ./setup_cron.sh
   ```

3. **Verify:**
   ```bash
   crontab -l
   ```

## Usage

**Manual recording:**
```bash
./record_npr.sh
```

**Output files:**
- `recordings/npr_atc_2025-09-16.mp3`
- `logs/recording.log`

## How It Works

**Daily recording:**
- Cron runs at 4:00 PM: `0 16 * * *`
- `timeout 7200` limits to 2 hours
- `wget` downloads stream to MP3 file
- Simple logging to track recordings

**Stream URL:**
`https://playerservices.streamtheworld.com/api/livestream-redirect/WUOMFM.mp3`

**File naming:**
`npr_atc_YYYY-MM-DD.mp3`

Total implementation: ~25 lines using only native Linux tools.