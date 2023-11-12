#!/bin/bash

# thanks mw
SCRIPT_DIR=$(dirname "$0")
SCRIPT_DIR=${SCRIPT_DIR:-"."}
. "$SCRIPT_DIR/lib/wax_common.sh"

cat logo.txt
echo "Dollar Store Shim OS™ Builder"

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

sleep 2 # arbitrary sleep for fun :trolllaugh:
echo "[*] Copying Bootloader..."
rsync -avh --progress ./rma/* rootmnt # this *should* merge the new files into the rootfs.

echo "[*] Mounting stateful partition"
mkdir -p statefulmnt
mount "${STATE}" statefulmnt

sleep 2 # yet another arbitrary sleep for fun :madTrolley:
echo "[*] Clearing stateful partition..."
rm -rf statefulmnt/* # absolutely fucking nuke the thing. going full oppenheimer mode here

sleep 3 # ok maybe not so arbitrary this time
echo "[*] Cleaning up..."
umount rootmnt
umount statefulmnt
rm -rf rootmnt
rm -rf statefulmnt

safesync

losetup -d "$LOOPDEV"

echo "[✓] All done!"