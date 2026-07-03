#!/usr/bin/env bash

# --- Configuration ---
set -eu pipefail
DISPLAY_NUM=":1"
RESOLUTION="1280x720"
XEPHYR_PKG="xorg-server-xephyr"

LOG_FILE="./build/temp/owm.log"
OWM_BIN="./owm"

make clean all
mkdir -p ./build/temp/ 

msg() {
    echo -e "\e[1;32m[+]\e[0m $1"
}

err() {
    echo -e "\e[1;31m[-]\e[0m $1" >&2
}

if [ -z "${DISPLAY:-}" ]; then
    err "No active X11 session detected (\$DISPLAY is empty)."
    msg "Run this inside a graphical environment."
    exit 1
fi

if ! command -v Xephyr &>/dev/null; then
    msg "Xephyr is not installed. Attempting to install via xbps-install..."
    if command -v sudo &>/dev/null; then
        sudo xbps-install -S "$XEPHYR_PKG"
    else
        err "sudo is not available. Please install '$XEPHYR_PKG' manually as root."
        exit 1
    fi
fi

if [ ! -x "$OWM_BIN" ]; then
    err "Could not find a valid executable: $OWM_BIN"
    exit 1
fi

cleanup() {
    msg "Cleaningup..."
    msg "Tearing down nested environment..."
    if [ -n "${XEPHYR_PID:-}" ] && kill -0 "$XEPHYR_PID" &>/dev/null; then
        kill "$XEPHYR_PID"
        exit 1
    fi
    exit 0
}
trap cleanup exit

msg "Starting Xephyr on display $DISPLAY_NUM ($RESOLUTION)..."
Xephyr -br -ac -screen "$RESOLUTION" "$DISPLAY_NUM" 2> "$LOG_FILE" &
XEPHYR_PID=$!
sleep 1
msg "Launching nested owm from: $OWM_BIN..."
msg "Logs and errors are at: $LOG_FILE"

DISPLAY="$DISPLAY_NUM" "$OWM_BIN" 2>> "$LOG_FILE"

msg "Nested owm session closed."
