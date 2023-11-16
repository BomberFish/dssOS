#!/bin/bash

handle_initial_options() {
     printf "Press any key within 3 seconds to drop to the shim's native shell..."
     read -s -r -t 3 -n 1 input

     if [[ -n $input || $input == " " || $input == "\n" ]]; then
          echo """

Dropping you to bash.
If at any point you want to enter the chroot, run the following commands:
mount /dev/sda13 /archmnt
mount --bind /dev /archmnt/layer/dev
mount --bind /proc /archmnt/layer/proc
chroot /archmnt/layer bash
     """
          open_shell
     else
          echo
          sleep 0.1
          echo
          echo "Booting dssOS..."
          sleep 0.25
          echo
          enter_chroot
     fi
}

open_shell() {
     while true; do
          /bin/bash -i # YOU CAN'T ESCAPE!
     done
}

handle_shutdown_options() {
     echo
     echo "[*] Shutting down chroot..."
     echo "[*] Unmounting /run/frecon..."
     umount /archmnt/layer/run/frecon
     sleep 0.3
     echo "[*] Unmounting /sys..."
     umount /archmnt/layer/sys
     sleep 0.2
     echo "[*] Unmounting /proc..."
     umount /archmnt/layer/proc
     sleep 0.4
     echo "[*] Unbinding /dev..."
     umount /archmnt/layer/dev
     sleep 0.5
     echo
     echo "It seems the chroot process ended. Would you like to (r)eboot, attempt to r(e)start the chroot, or enter a (s)hell? [r/e/s]"
     read -r -p "> " -n 1 input
     printf "\n\n"
     if [[ $input == "r" ]]; then
          echo "[*] Rebooting..."
          reboot now
     elif [[ $input == "e" ]]; then
          echo "[*] Attempting to restart chroot..."
          enter_chroot
     elif [[ $input == "s" ]]; then
          echo "[*] Entering shell..."
          open_shell
     else
          echo "[*] Unknown command. Entering shell..."
          open_shell
     fi
}

enter_chroot() {
     sleep 0.75
     echo "[*] Mounting user partition..."
     mount /dev/sda13 /archmnt # no way a chromebook has a sata drive
     sleep 0.5
     echo "[*] Binding /dev..."
     mount --bind /dev /archmnt/layer/dev
     sleep 0.4
     echo "[*] Mounting /proc..."
     mount -t proc /proc /archmnt/layer/proc
     sleep 0.2
     echo "[*] Mounting /sys..."
     mount -t sysfs /sys /archmnt/layer/sys
     echo "[*] Mounting /run/frecon..."
     mount --bind /run/frecon /archmnt/layer/run/frecon
     sleep 0.3
     echo "[*] Entering chroot"
     echo
     chroot /archmnt/layer bash
     handle_shutdown_options
}

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
printf "\ndssOS Bootloader v0.1\n"

handle_initial_options
