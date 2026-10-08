#!/bin/bash
echo "Server Status"

system_info(){
    echo "System Information"
    echo "Hostname: $(hostname)"

    if [ -r /etc/os-release ]; then
        echo "OS  : $(. /etc/os-release && echo ""$PRETTY_NAME)"
    fi


    echo "User    : $(whoami)"
    echo "Kernel  : $(uname -r)"
    echo "Arch    : $(uname -m)"
    echo "Date    : $(date)"
    echo "Uptime  : $(uptime)"
}
system_info
