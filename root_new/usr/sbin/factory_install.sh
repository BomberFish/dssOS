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

/bin/bash -i