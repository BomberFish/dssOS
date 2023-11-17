CC ?= gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Ofast -static

default: bootloader_full

clean:
	rm -f dssos_bootloader dssos_motd

everything: bootloader_full motd_full

bootloader_full: bootloader optimize_bootloader print_blsize copy_bootloader
motd_full: motd optimize_motd print_motdsize copy_motd

bootloader: bootloader.c
	$(CC) $(CFLAGS) -o dssos_bootloader bootloader.c

optimize_bootloader:
	strip dssos_bootloader
	upx --best dssos_bootloader

copy_bootloader:
	cp dssos_bootloader ../root_new/bin/dssos_bootloader

print_blsize:
	du -h dssos_bootloader

motd: motd.c
	$(CC) $(CFLAGS) -o dssos_motd motd.c

optimize_motd:
	strip dssos_motd
	upx --best dssos_motd

copy_motd:
	cp dssos_motd ../root_new/bin/dssos_motd

print_motdsize:
	du -h dssos_motd