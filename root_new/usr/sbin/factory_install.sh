#!/bin/bash
umask 022

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

echo "[*] Mounting user partition..."
mount /dev/sda13 /archmnt # no way a chromebook has a sata drive
echo "[*] Binding /dev..."
mount --bind /dev /archmnt/layer/dev
echo "[*] Binding /proc..."
mount --bind /proc /archmnt/layer/proc
echo "[*] Entering chroot"
chroot /archmnt/layer bash