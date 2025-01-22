#ifndef MISC_H_
#define MISC_H_

#include "../raylib/raylib.h"
#include "include/lognest.h"
#include <stdlib.h>
#include <time.h>

inline void troll(void);

inline void raylib_pre(void) {

    srand(time(NULL));
    int random = rand() % 185 + 1;

    if (random == 1) {
        troll();
    }

    SetWindowState(FLAG_MSAA_4X_HINT);
    SetWindowState(FLAG_VSYNC_HINT);
}

inline void raylib_config(void) {

    lognest_trace("[Raylib] Window opened.");
    SetTraceLogLevel(LOG_NONE);

    SetTargetFPS(60);
    ToggleFullscreen();
    lognest_trace("[Raylib] Enabled Fullscreen.");

    InitAudioDevice();
}

inline const char *bool_to_string(bool value) {
    return value ? "true" : "false";
}

inline bool file_exists(const char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        // file exists
        return true;
    } else {
        return false;
    }
    // if something goes horribly wrong, just say the file dosen't exist.
    return false;
}

inline void troll(void) {

    lognest_error("[SYSTEM] ATTENTION:\n"
                  "                       ;:::::;                       \n"
                  "                    :::::::::::::.:;                  \n"
                  "                    ::::::::::::::::::.               \n"
                  "                  .::::::::::::::::::::::             \n"
                  "      :       :..................::::::::             \n"
                  " ..:....:::::::::::...........:...::::::.....         \n"
                  "::.:::::::::.............:::::::::::::::::.:::::;:    \n"
                  "::....:.....::::....::...:::::::.........:::::::::::  \n"
                  "::....::::::::::::::::::::::::..............:::::::+  \n"
                  ":::::.::::::::::::::::::::::::::::...........::::;X+  \n"
                  "::::::;;::::::::::......:::::::::::::.......::::$&&+  \n"
                  "  ::::;;;+++;:::::...::::::::;;;:::::.....::::::$$x:  \n"
                  "  ::::::;+++;:::::...::::;::::;;;::::::::::::::;:::;  \n"
                  "  :::::::::;;::::......::;;:::;:::::::::::::;;;;:::;  \n"
                  "  .:::..::::;;;:::......:::;X$::::::::::::::;;;;:::.  \n"
                  "  ;:::...:::;;;;::::....:::$&$:::;;;;:::::::xX$x::::  \n"
                  "  ;;;:::.:;;;;;;::::::::..:Xx;:.:;;;;;;xx:::X$$x:::;  \n"
                  "  ;;;:::::;;::;;;;;;;:::...:::..:XxX&&&&x:::X$$x:::;  \n"
                  "  ;;;;:::::::..:;;;;;::::..:::;;;&&&&&&&XX&&&&$x::::  \n"
                  "  :;;;::::::...:::;;;:::::::::;;;&&&&&&&&&&&&$X+;;;:  \n"
                  "  .;;;:::::::::.::;;;;;;:::::::::;&&&&&&&&&&xxx+;;;.  \n"
                  "  .;;;;;;;:::::...:::;;;:::::::::;&&&&&&&&&&xxx+;;;.  \n"
                  "  .+++;;;;;::::...::::;;:::::::::;xxx$&&&&&$$$$x;;;.  \n"
                  "  .+++;;;;;;::::::::::;:::::::;;;;xxx$&&$XXX$&$x;;+.  \n"
                  "    ;;;;;;;;;;;::::::::::::::;;;;;X$&&&&$XXX$xx+;;    \n"
                  "         :;;;;;;;:::::::;;;;;;;;;;&&&&&&Xxxx++++      \n"
                  "             ;:;;::::;;;;;;;;;;;;;&&Xxxxxxxx+++;      \n"
                  "                  :::;;;++++++;;;+++++xxxx++          \n"
                  "                       :++++++;. .+++++               \n"
                  "                          ;;      +                   \n");
    lognest_error("[SYSTEM] Thanks for your attention.");
}

#endif // !MISC_H_
