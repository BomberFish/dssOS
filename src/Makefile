CC ?= gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Oz -static

default: bootloader

bootloader: bootloader.c
	$(CC) $(CFLAGS) -o dssos_bootloader bootloader.c

motd: motd.c
	$(CC) $(CFLAGS) -o dssos_motd motd.c