#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int random_int(int max) {
    return rand() % max;
}

int main(void) {
    const char *messages[] = {"Nine parts skiddery", "It's so sad Steve Jobs died of ligma", "Wait, it's all TerraOS?", "Thanks for giving me the planet, fricking idiot.", "sh2mmer (pronounced sh-tumor)", "Shoutouts to lenovo-driver-download.com", "What's up sysadmins?", "Take me to your leader", "Bite me!", "BORN TO SKID\nLTBEEF IS A FUCK\nShim Em All 1989\nI am proxy man\n410,757,864,530 DEAD CHROMEBOOKS", "Brought to you by the Whitelist guy", "Remember to remove your enrollment chip before leaving\n(for legal reasons that's a joke)"};
    int len = sizeof(messages) / sizeof(messages[0]);
    srand(time(NULL)); // seed the random number generator
    int rand = random_int(len);
    const char *message = messages[rand];
    printf("%s\n", message);
    return 0;
}