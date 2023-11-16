#include <stdio.h>
#include <time.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <termios.h>
#include <unistd.h>

// TODO: Get bash job control working

int main(void) {
    int exit = 0;
    printf("       __             //___  _____\n  ____/ /_____ _____ // __ \/ ___/\n / __  // ___// ___/// / / /\__ \ \n/ /_/ /(__  )(__  )// /_/ /___/ / \n\____//____//____///\____//____/\nThe Dollar Store Shim Operating System\n");
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
        printf("Dropping you to bash.\nIf at any point you want to enter the chroot, run the following commands:\nmount /dev/sda13 /archmnt\nmount --bind /dev /archmnt/layer/dev\nmount --bind /proc /archmnt/layer/proc\nmount --bind /run/frecon /archmnt/layer/run/frecon\nchroot /archmnt/layer bash\n");
        run("while true; do /bin/bash -i; done");
    } else {
        printf("\nBooting dssOS...\n");
        exit = enter_chroot();
        run_chroot("bash -i", "/archmnt/layer");
    }

    // restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return exit;
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
        open_shell();
    default:
        printf("\n[*] Invalid option. Entering shell.\n");
        open_shell();
    }
}

void open_shell(void) {
    run("/bin/bash -c 'while true; do /bin/bash -i; done'");
}

int start_chroot(void) {
    printf("[*] Starting chroot...\n");
    enter_chroot();
    return run_chroot("bash", "/archmnt/layer");
}

int enter_chroot(void) {
    // using run() until i can find a way to do this in c
    printf("[*] Preparing mount point...\n");
    run("mkdir -p /archmnt");
    syscall(SYS_mkdir, "/archmnt", 0777);

    printf("[*] Mounting user partition...\n");
    syscall(SYS_mount, "/dev/sda13", "/archmnt", "ext4", 0, NULL);

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

int run(char *cmd) {
    return system("/bin/bash -c '%s'", cmd);
}

int run_chroot(char *cmd, char *chroot) {
    return system("%s/bin/bash -c '%sZ'", chroot, cmd);
}

void printMOTD(void) {
    const char *messages[] = {"Nine parts skiddery", "It's so sad Steve Jobs died of ligma", "Wait, it's all TerraOS?", "Thanks for giving me the planet, fricking idiot.", "sh2mmer (pronounced sh-tumor)", "Shoutouts to lenovo-driver-download.com", "What's up sysadmins?", "Bite me!", "BORN TO SKID\nLTBEEF IS A FUCK\nShim Em All 1989\nI am proxy man\n410,757,864,530 DEAD CHROMEBOOKS", "Brought to you by the Whitelist guy", "Remember to remove your enrollment chip before leaving\n(for legal reasons that's a joke)", "We've disssssabled dssOS sssservicess", "say gex", "Oh? On dssOS?", "Go buy Picasso at repo.sourceloc.net", "Lucky for you, it's snack time", "I want a frickin' ninja star!", "Somehow worse than ALIAS", "Just pop it in your mouth", "Now in glorious C!"};
    int len = sizeof(messages) / sizeof(messages[0]);
    srand(time(NULL)); // seed the random number generator
    int rand = random() % len;
    const char *message = messages[rand];
    printf("%s\n", message);
    return 0;
}