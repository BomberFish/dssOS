#!/bin/bash

# thanks mw
SCRIPT_DIR=$(dirname "$0")
SCRIPT_DIR=${SCRIPT_DIR:-"."}
. "$SCRIPT_DIR/lib/wax_common.sh"

set -e
if [ "$EUID" -ne 0 ]; then
	echo "[!!!] Please run this script as root!"
	exit 1
fi

if [ -z "$1" ]; then
    echo "[!!!] No shim supplied!"
	exit 1
fi


SHIM=$1

if [[ ! -f $SHIM ]]; then
	echo "[!!] The file path specified does not exist!"
	exit 1
fi


echo "[*] Creating loop device..."
LOOPDEV=$(losetup -f)
losetup -P "${LOOPDEV}" "${SHIM}"
ROOTA="${LOOPDEV}p3"
STATE="${LOOPDEV}p1"

echo "[*] Making ROOT-A writable..."
enable_rw_mount "${ROOTA}"

echo "[*] Mounting ROOT-A..."
mkdir -p rootmnt
mount "${ROOTA}" rootmnt

echo "[*] Mounting stateful partition"
mkdir -p statefulmnt
mount "${STATE}" statefulmnt

echo
echo "[✓] All done! Make sure to manually detach any loop devices and unmount any mounted partitions when you're finished."