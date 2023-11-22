#!/bin/bash

/bin/dssos_bootloader

echo
echo "The bootloader seems to have exited. If you wish to restart it, please run /bin/dssos_bootloader manually."
while true; do
    /bin/bash
done