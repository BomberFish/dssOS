#!/bin/bash
umask 022
mkdir -p /archmnt

echo """
       __             //___  _____
  ____/ /_____ _____ // __ \/ ___/
 / __  // ___// ___/// / / /\__ \ 
/ /_/ /(__  )(__  )// /_/ /___/ / 
\____//____//____///\____//____/  
                                 
The Dollar Store Shim Operating System
"""
/bin/dssos_motd # print a funny message
echo

enter_chroot() {
echo "[*] Mounting user partition..."
mount /dev/sda13 /archmnt # no way a chromebook has a sata drive
echo "[*] Binding /dev..."
mount --bind /dev /archmnt/layer/dev
echo "[*] Mounting /proc..."
mount -t proc /proc /archmnt/layer/proc
echo "[*] Mounting /sys..."
mount -t sysfs /sys /archmnt/layer/sys
echo "[*] Entering chroot"
chroot /archmnt/layer bash
}

echo "dssOS Bootloader v0.1"
printf "Press any key within 5 seconds to drop to the shim's native shell..."
read -s -r -t 5 -n 1 input

if [[ -z $input ]]; then
     echo "Booting dssOS..."
     enter_chroot
else 
     echo """

Dropping you to bash.
If at any point you want to enter the chroot, run the following commands:
mount /dev/sda13 /archmnt
mount --bind /dev /archmnt/layer/dev
mount --bind /proc /archmnt/layer/proc
chroot /archmnt/layer bash
     """
     /bin/bash -i
fi