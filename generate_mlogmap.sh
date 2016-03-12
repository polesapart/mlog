#!/bin/bash

# Extracts MLOG symbols and their addresses from a given executable.

if [ -z "$1" ]; then
    echo "Usage: $0 <executable>"
    exit 1
fi

nm --numeric-sort "$1" | grep -E '^[0-9a-fA-F]+\s+.*MLOG_.*___$' | sed -r -e 's/^([0-9a-fA-F]+)\s+d\s+.*(MLOG_.*___)$/\1 \2/'
