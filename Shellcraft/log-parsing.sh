#!/bin/bash
# ==============================================================================
# SCRIPT NAME    : Nginx Log Parser & Behavioral Analysis Tool
# PURPOSE        : Filter, extract, and count unique patterns/signatures from
#                  Nginx access logs for security behavioral analysis and threat
#                  mapping.
# SCOPE          : Chains core Linux utilities (grep, awk, sort, tail, head, uniq)
#                  to produce context-aware analytical output across target perimeters.
# COMPATIBILITY  : POSIX-compliant Shell / Bash 4.0+
# DEPENDENCIES   : coreutils (grep, awk, sort, uniq, tail, head)
# ==============================================================================
#
# ------------------------------------------------------------------------------
# NGINX COMBINED LOG FORMAT SPECIFICATION
# ------------------------------------------------------------------------------
# Default format string:
# '$remote_addr - $remote_user [$time_local] "$request" $status $body_bytes_sent "$http_referer" "$http_user_agent"'
#
# SAMPLE LOG ENTRY:
# 57.94.25.192 - - [01/Oct/2026:00:00:30 +0300] "POST /static/app.js HTTP/1.1" 200 51956 "-" "python-requests/2.31.0"
#
# FIELD MAPPING & AWK COLUMN INDEXES:
# $1  : $remote_addr     -> Client IP address (Source IP of request)
# $2  : identd           -> Legacy RFC 1413 identifier (almost always '-')
# $3  : $remote_user     -> Authenticated user (via HTTP Basic Auth, else '-')
# $4  : $time_local (1)  -> Date/Time stamp opening bracket + timestamp [DD/Mon/YYYY:HH:MM:SS
# $5  : $time_local (2)  -> Timezone offset string + bracket (+0300])
# $6  : $request (1)     -> HTTP Method (GET, POST, PUT, DELETE, HEAD, OPTIONS)
# $7  : $request (2)     -> Request URI Path + Query String (e.g., /static/app.js, /.env)
# $8  : $request (3)     -> HTTP Protocol Version (e.g., HTTP/1.1", HTTP/2.0)
# $9  : $status          -> HTTP Response Status Code (2xx, 3xx, 4xx, 5xx)
# $10 : $body_bytes_sent -> Response payload size in bytes (excluding HTTP headers)
# $11 : $http_referer    -> HTTP Referer header (originating page URL or '-')
# $12+: $http_user_agent -> User-Agent string identifying client software/tool
# ------------------------------------------------------------------------------
#
# ------------------------------------------------------------------------------
# THREAT & BEHAVIORAL ANALYSIS METRICS
# ------------------------------------------------------------------------------
# 1. WHO (Identity & Tooling):
#    - Client IP ($1) & User-Agent ($12+): Identifies automated scanners, bots,
#      custom scripts (e.g., python-requests), or malicious client infrastructure.
#
# 2. WHAT (Target Intent):
#    - HTTP Method ($6) & URI Path ($7): Differentiates legitimate application
#      traffic from reconnaissance, force browsing, or credential stuffing (e.g., probing /.env).
#
# 3. WHAT HAPPENED (Server Reaction):
#    - Status Code ($9) & Data Size ($10): Detects successful exploitation (200),
#      failed brute-force/dirbusting (404/401/403), or data exfiltration footprints.
#
# 4. WHEN (Temporal Patterning):
#    - Timestamp ($4, $5): Used to construct incident chronologies, map burst activity,
#      and correlate concurrent log entries during triage.
#
# 5. Resources for setting up the Test file are in [https://github.com/systems-jackal/Jackal-Citadel/blob/main/Test-Lab/access.log]
# ------------------------------------------------------------------------------

