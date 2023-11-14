#!/bin/bash

# thanks mw
SCRIPT_DIR=$(dirname "$0")
SCRIPT_DIR=${SCRIPT_DIR:-"."}
. "$SCRIPT_DIR/lib/wax_common.sh"

echo
cat logo.txt
echo "  Dollar Store Shim OS™ Builder"
echo

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

echo "[*] Creating user partition..."

dd if=/dev/zero bs=1G status=progress count=6 >>"${SHIM}" # 6gb should be enough for anyone, right?
echo -ne "\a"
# Fix corrupt gpt
echo "[*] Fixing up image..."
fdisk "${SHIM}" <<EOF
n



w
EOF	

echo "[*] Creating loop device..."
LOOPDEV=$(losetup -f)
losetup -P "${LOOPDEV}" "${SHIM}"
ROOTA="${LOOPDEV}p3"
USR="${LOOPDEV}p13"

echo "[*] Formatting user partition..."
mkfs.ext2 -L arch "${LOOPDEV}p13"

cleanup() {
	echo "[*] Cleaning up..."
	set -x
	umount rootmnt
	umount usrmnt
	rm -rf rootmnt
	rm -rf usrmnt

	safesync

	losetup -d "$LOOPDEV"
	set +x
}

echo "[*] Making ROOT-A writable..."
enable_rw_mount "${ROOTA}"

echo "[*] Mounting ROOT-A..."
mkdir -p rootmnt
mount "${ROOTA}" rootmnt
sleep 2 # arbitrary sleep for fun :trolllaugh:

echo "[*] Copying Bootloader..."
rsync -avh --progress ./root_new/* rootmnt || cleanup # this *should* merge the new files into the rootfs.

echo "[*] Mounting user partition"
mkdir -p usrmnt
mount "${USR}" usrmnt
sleep 2 # yet another arbitrary sleep for fun :madTrolley:

echo "[*] Copying base system. This may take a while."
rsync -ah --progress ./basesystem/layer usrmnt || cleanup

sleep 3 # ok maybe not so arbitrary this time
cleanup

echo "[✓] All done!"
