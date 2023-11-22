#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/syscall.h>

int open_shell(void) {
    char *newargv[] = {"/bin/bash", NULL};
   if (execv(newargv[0], newargv) == -1) {
         printf("Failed to open shell. Exiting.\n");
         return 1;
    } else {
         return 0;
   }
}

int enter_chroot(void) {
    printf("[*] Preparing mount point...\n");
    syscall(SYS_mkdir, "/archmnt", 0777);

    printf("[*] Mounting user partition...\n");
    syscall(SYS_mount, "/dev/sda13", "/archmnt", "ext4", 0, NULL);
    syscall(SYS_chdir, "/archmnt/layer"); // idk why but at first the chroot decides to stay in the shim's root (?????)

    printf("[*] Binding /dev...\n");
    syscall(SYS_mount, "/dev", "/archmnt/layer/dev", NULL, 0x1000, NULL); // thanks https://pkg.go.dev/syscall#pkg-constants

    printf("[*] Mounting /proc...\n");
    syscall(SYS_mount, "/proc", "/archmnt/layer/proc", "proc", 0, NULL);

    printf("[*] Mounting /sys...\n");
    syscall(SYS_mount, "/proc", "/archmnt/layer/proc", "proc", 0, NULL);

    printf("[*] Mounting /run/frecon...\n");
    syscall(SYS_mount, "/run/frecon", "/archmnt/layer/run/frecon", NULL, 0x1000, NULL);

    printf("[*] Entering chroot...\n");
    return syscall(SYS_chroot, "/archmnt/layer");
}

void handleCrash(void) {
    printf("[*] Shutting down chroot...\n");
    printf("[*] Unmounting /dev...\n");
    syscall(SYS_umount2, "/archmnt/layer/dev");
    printf("[*] Unmounting /proc...\n");
    syscall(SYS_umount2, "/archmnt/layer/proc");
    printf("[*] Unmounting /sys...\n");
    syscall(SYS_umount2, "/archmnt/layer/sys");
    printf("[*] Unmounting /run/frecon...\n");
    syscall(SYS_umount2, "/archmnt/layer/run/frecon");

    printf("It seems the chroot process ended. Would you like to (r)eboot, attempt to r(e)start the chroot, or enter a (s)hell? [r/e/s]");
    char input = getchar();

    switch (input) {
    case 'r':
        printf("\n[*] Rebooting\n");
    case 'e':
        printf("\n[*] Attempting to restart chroot\n");
    case 's':
        printf("\n[*] Entering shell\n");
        if (open_shell() == 1) {
                printf("Failed to open shell. Press Refresh+Power to reboot into CrOS.\n");
            } 
    default:
        printf("\n[*] Invalid option. Entering shell.\n");
        if (open_shell() == 1) {
            printf("Failed to open shell. Press Refresh+Power to reboot into CrOS.\n");
        } 
    }
}

void printMOTD(void) {
    const char *messages[] = {"Nine parts skiddery", "It's so sad Steve Jobs died of ligma", "Wait, it's all TerraOS?", "Thanks for giving me the planet, fricking idiot.", "sh2mmer (pronounced sh-tumor)", "Shoutouts to lenovo-driver-download.com", "What's up sysadmins?", "Bite me!", "BORN TO SKID\nLTBEEF IS A FUCK\nShim Em All 1989\nI am proxy man\n410,757,864,530 DEAD CHROMEBOOKS", "Brought to you by the Whitelist guy", "Remember to remove your enrollment chip before leaving\n(for legal reasons that's a joke)", "We've disssssabled dssOS sssservicess", "say gex", "Oh? On dssOS?", "Go buy Picasso at repo.sourceloc.net", "Lucky for you, it's snack time", "I want a frickin' ninja star!", "Somehow worse than ALIAS", "Just pop it in your mouth", "Now in glorious C!"};
    int len = sizeof(messages) / sizeof(messages[0]);
    srand(time(NULL)); // seed the random number generator
    int random = rand() % len;
    const char *message = messages[random];
    printf("%s\n", message);
}

int main(int argc, char *argv[]) {
    int exit = 0;
    puts(
        "         __             //___  _____\n"
        "    ____/ /_____ _____ // __ \\/ ___/\n"
        "   / __  // ___// ___/// / / /\\__ \\ \n"
        "  / /_/ /(__  )(__  )// /_/ /___/ / \n"
        "  \\____//____//____///\\____//____/  \n\n"
        "The Dollar Store Shim Operating System\n"
    );
    printMOTD();
    printf("\ndssOS Bootloader v0.2\n");

    printf("Press any key within 3 seconds to drop to the shim's native shell...\n");

    // set up terminal to read input without waiting for newline
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    // set up select to wait for input or timeout
    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    int retval = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
        perror("select()");
    } else if (retval) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); // restore terminal to sane state
        printf("Dropping you to bash.\nIf at any point you want to enter the chroot, run the following commands:\nmount /dev/sda13 /archmnt\nmount --bind /dev /archmnt/layer/dev\nmount --bind /proc /archmnt/layer/proc\nmount --bind /run/frecon /archmnt/layer/run/frecon\nchroot /archmnt/layer bash\n");
        if (open_shell() == 1) {
            tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); // restore terminal to sane state
            printf("Failed to open shell. Press Refresh+Power to reboot into CrOS.\n");
        } 
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); // restore terminal to sane state
        printf("\nBooting dssOS...\n");
        while (1) {
            exit = enter_chroot();
            if (open_shell() == 1) {
                tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); // restore terminal to sane state
                printf("Failed to open shell. Press Refresh+Power to reboot into CrOS.\n");
            } 
            handleCrash();
        }
    }

    return exit;
}