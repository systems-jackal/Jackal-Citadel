#!/usr/bin/env bash
# ------------------------------------------------------------------------------
# SHEBANG: tells the system which interpreter runs this file when it is executed
# directly (./log-parsing.sh). "env" locates bash wherever it is installed, which
# is more portable than hard-coding /bin/bash.
# ------------------------------------------------------------------------------

# ==============================================================================
# SCRIPT NAME    : Nginx Log Parser & Behavioral Analysis Tool
# PURPOSE        : Filter, extract, and count unique patterns/signatures from
#                  Nginx access logs for security behavioral analysis and threat
#                  mapping.
# SCOPE          : Chains core Linux utilities (grep, awk, sort, uniq, tail, head)
#                  to produce context-aware analytical output.
# USAGE          : ./log-parsing.sh [path/to/access.log]
#                  With no argument, the bundled sample (Test-Lab/access.log) is used.
# COMPATIBILITY  : Bash 4.0+
# DEPENDENCIES   : coreutils (grep, awk, sort, uniq, tail, head, wc, tr)
# ==============================================================================
#
# ------------------------------------------------------------------------------
# NGINX COMBINED LOG FORMAT SPECIFICATION
# ------------------------------------------------------------------------------
# Format string:
# '$remote_addr - $remote_user [$time_local] "$request" $status $body_bytes_sent "$http_referer" "$http_user_agent"'
#
# SAMPLE LOG ENTRY:
# 57.94.25.192 - - [01/Oct/2026:00:00:30 +0300] "POST /static/app.js HTTP/1.1" 200 51956 "-" "python-requests/2.31.0"
#
# FIELD MAPPING (awk splits on whitespace, so the quoted request spans 3 columns):
# $1  : $remote_addr     -> Client IP address
# $2  : identd           -> Legacy identifier (almost always '-')
# $3  : $remote_user     -> Authenticated user via HTTP Basic Auth, else '-'
# $4  : $time_local (1)  -> "[01/Oct/2026:00:00:30"  (note the leading '[')
# $5  : $time_local (2)  -> "+0300]"                 (timezone, trailing ']')
# $6  : $request (1)     -> "\"POST"  HTTP method WITH a leading double quote
# $7  : $request (2)     -> Request path + query string (/static/app.js, /.env)
# $8  : $request (3)     -> "HTTP/1.1\""  protocol WITH a trailing double quote
# $9  : $status          -> HTTP response status code (2xx, 3xx, 4xx, 5xx)
# $10 : $body_bytes_sent -> Response body size in bytes (headers excluded)
# $11 : $http_referer    -> Referer header, or "-"
# $12+: $http_user_agent -> Client software (browser, bot, scanner tool)
#
# TIP: $4, $6 and $8 carry stray punctuation. Strip it (tr -d '[' / tr -d '"')
#      before comparing values, or match with a regex instead of ==.
# ------------------------------------------------------------------------------
#
# ------------------------------------------------------------------------------
# ANALYSIS QUESTIONS THIS SCRIPT WILL EVENTUALLY ANSWER
# ------------------------------------------------------------------------------
# WHO         : Client IP ($1) and User-Agent ($12+)
# WHAT        : Method ($6) and path ($7): normal traffic vs probing
# WHAT HAPPENED: Status ($9) and size ($10): did the probe succeed?
# WHEN        : Timestamp ($4): timelines and bursts
#
# Sample data: https://github.com/systems-jackal/Jackal-Citadel/blob/main/Test-Lab/access.log
# ------------------------------------------------------------------------------


# ==============================================================================
# STRICT MODE: fail loudly instead of continuing after something goes wrong
# ==============================================================================
# -e          : exit immediately if a command fails
# -u          : treat an unset variable as an error (catches typos like $LOGG)
# -o pipefail : a pipeline fails if ANY stage fails, not just the last one
set -euo pipefail


# ==============================================================================
# PATHS: resolved from the script's own location, never the caller's directory.
# This keeps the repo portable: it works wherever it is cloned.
# ==============================================================================
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"   # .../Shellcraft
CITADEL_DIR="$(dirname "$SCRIPT_DIR")"                       # .../Jackal-Citadel
LAB_DIR="$CITADEL_DIR/Test-Lab"                              # sample logs
REPORT_DIR="$CITADEL_DIR/Reports"                            # generated output (used in later steps)

# ${1:-default}: use the first argument if given, otherwise the default.
LOG="${1:-$LAB_DIR/access.log}"


# ==============================================================================
# STEP 1: VALIDATE INPUT
# ==============================================================================
# Pattern:  [[ test ]] || { message; exit 1; }
#   - If the test is TRUE, nothing happens and the script continues.
#   - If the test is FALSE, the "||" branch runs: print an error and stop.
#
# Order matters: check "is it a readable file?" first, THEN "is it non-empty?".
# Checking emptiness first would give a misleading message for a missing file.
#
#   -f : exists and is a REGULAR file (rejects directories such as /var/log)
#   -r : readable by the current user
#   -s : exists and has a size greater than zero
#
# ">&2" sends the message to stderr, so errors never pollute redirected reports.
# "exit 1" returns a non-zero code so callers (scripts, CI) can detect failure.
validate_input() {
  [[ -f "$LOG" ]] || { echo "Error: not a regular file: $LOG" >&2; exit 1; }
  [[ -r "$LOG" ]] || { echo "Error: cannot read file: $LOG" >&2; exit 1; }
  [[ -s "$LOG" ]] || { echo "Error: file is empty: $LOG" >&2; exit 1; }
}


# ==============================================================================
# HELPER: print a section heading so the report is easy to scan
# ==============================================================================
section() {
  printf '\n== %s ==\n' "$1"
}


# ==============================================================================
# STEP 2: BASELINE SUMMARY
# ==============================================================================
baseline() {
  section "BASELINE"

  # "local" keeps these variables inside the function.
  # Declaring and assigning on SEPARATE lines preserves the command's exit status
  # (local x=$(cmd) would hide a failure from "set -e").
  local total unique first last

  total=$(wc -l < "$LOG")                                   # lines = requests
  unique=$(awk '{print $1}' "$LOG" | sort -u | wc -l)      # distinct client IPs
  first=$(head -n 1 "$LOG" | awk '{print $4}' | tr -d '[') # first timestamp, '[' removed
  last=$(tail -n 1 "$LOG"  | awk '{print $4}' | tr -d '[') # last timestamp,  '[' removed

  printf '%-16s: %s\n' "Log file"       "$LOG"
  printf '%-16s: %s\n' "Total requests" "$total"
  printf '%-16s: %s\n' "Unique IPs"     "$unique"
  printf '%-16s: %s\n' "First request"  "$first"
  printf '%-16s: %s\n' "Last request"   "$last"
}


# ==============================================================================
# STEP 3: STATUS CODE BREAKDOWN
# ==============================================================================
# Pipeline: extract status -> group identical values -> count -> rank by count
#   sort     : uniq only collapses ADJACENT duplicates, so sort first
#   uniq -c  : prefix each distinct value with its count
#   sort -rn : -n numeric, -r reverse (largest first)
status_breakdown() {
  section "STATUS CODES (count, code)"
  awk '{print $9}' "$LOG" | sort | uniq -c | sort -rn
}


# ==============================================================================
# STEP 4: TOP TALKERS (most active client IPs)
# ==============================================================================
# Same "top N" pipeline as above, trimmed with head.
top_ips() {
  section "TOP 10 CLIENT IPs (count, ip)"
  awk '{print $1}' "$LOG" | sort | uniq -c | sort -rn | head -n 10
}


# ==============================================================================
# TODO: later steps (add one function at a time, test each before moving on)
#   STEP 5 : detect probe paths      (/.env, /.git/, /phpmyadmin, /../ ...)
#   STEP 6 : detect attack-tool user agents (sqlmap, nikto, nmap)
#   STEP 7 : success check           (did any probe return 200?)
#   STEP 8 : per-IP risk summary     (requests, 4xx count, probe hits)
#   STEP 9 : server error report     (5xx per hour)
#   STEP 10: write a report to $REPORT_DIR and a block list of IPs
# ==============================================================================


# ==============================================================================
# MAIN: the order of execution lives in one place
# ==============================================================================
main() {
  validate_input
  echo "Nginx access.log analysis: behavioral and log patterns"
  baseline
  status_breakdown
  top_ips
}

# "$@" forwards all command-line arguments to main.
main "$@"
