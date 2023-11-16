#include <stdio.h>
#include <time.h>

int main(void) {
    int exit = 0;
    printf("       __             //___  _____\n  ____/ /_____ _____ // __ \/ ___/\n / __  // ___// ___/// / / /\__ \ \n/ /_/ /(__  )(__  )// /_/ /___/ / \n\____//____//____///\____//____/\nThe Dollar Store Shim Operating System\n");
    printMOTD();
    printf("\ndssOS Bootloader v0.1\n");

    printf("Press any key within 3 seconds to drop to the shim's native shell...\n");
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 3) {
        if (getchar() != EOF) {
            exit = enter_chroot();
            
            break;
        } else {
            system("/bin/bash -c 'while true; do /bin/bash -i; done'");
        }
    }
    return exit;
}

int enter_chroot() {
    // using run() until i can find a way to do this in c
    printf("[*] Preparing mount point...\n");
    run("mkdir -p /archmnt");
    
    printf("[*] Mounting user partition...\n");
    run("mount /dev/sda13 /archmnt");

    printf("[*] Binding /dev...\n");
    run("mount --bind /dev /archmnt/layer/dev");
    
    printf("[*] Mounting /proc...\n");
    run("mount -t proc /proc /archmnt/layer/proc");

    printf("[*] Mounting /sys...\n");
    run("mount -t sysfs /sys /archmnt/layer/sys");

    printf("[*] Mounting /run/frecon...\n");
    run("mount --bind /run/frecon /archmnt/layer/run/frecon");

    printf("[*] Entering chroot...\n");
    return chroot("/archmnt/layer");
}

int run(char *cmd) {
    return system("/bin/bash -c '%s'", cmd);
}

int run_chroot(char *cmd, char *chroot) {
    return system("%s/bin/bash -c '%d'", chroot, cmd);
}

void printMOTD(void) {
    const char *messages[] = {"Nine parts skiddery", "It's so sad Steve Jobs died of ligma", "Wait, it's all TerraOS?", "Thanks for giving me the planet, fricking idiot.", "sh2mmer (pronounced sh-tumor)", "Shoutouts to lenovo-driver-download.com", "What's up sysadmins?", "Bite me!", "BORN TO SKID\nLTBEEF IS A FUCK\nShim Em All 1989\nI am proxy man\n410,757,864,530 DEAD CHROMEBOOKS", "Brought to you by the Whitelist guy", "Remember to remove your enrollment chip before leaving\n(for legal reasons that's a joke)", "We've disssssabled dssOS sssservicess", "say gex", "Oh? On dssOS?", "Go buy Picasso at repo.sourceloc.net", "Lucky for you, it's snack time", "I want a frickin' ninja star!", "Somehow worse than ALIAS", "Just pop it in your mouth"};
    int len = sizeof(messages) / sizeof(messages[0]);
    srand(time(NULL)); // seed the random number generator
    int rand = random() % len;
    const char *message = messages[rand];
    printf("%s\n", message);
    return 0;
}