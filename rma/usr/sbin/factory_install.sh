#!/bin/bash

set -e
umask 022

echo """
       __             //___  _____
  ____/ /_____ _____ // __ \/ ___/
 / __  // ___// ___/// / / /\__ \ 
/ /_/ /(__  )(__  )// /_/ /___/ / 
\__,_//____//____///\____//____/  
                                 
The Dollar Store Shim Operating System
"""

su -P -c '/bin/bash -i' # todo: work on bootloader!