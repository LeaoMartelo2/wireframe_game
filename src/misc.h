#ifndef MISC_H_
#define MISC_H_

#include <stddef.h>

#include "../raylib/raylib.h"
#include "include/lognest.h"
#include <stdlib.h>
#include <time.h>


#define UNUSED(x) (void)(x)

#define TODO(x) do {printf("\n\n\n'%s' AT: \n %s:%d\n", x, __FILE__, __LINE__); abort(); } while(0)


inline void troll(void);

static inline void raylib_pre(void) {

    lognest_trace("|=====================-Wireframe Engine Start-=====================|\n");

    srand(time(NULL));
    int random = rand() % 185 + 1;

    if (random == 1) {
        troll();
    }

    SetWindowState(FLAG_MSAA_4X_HINT);
    SetWindowState(FLAG_VSYNC_HINT);
}

inline void raylib_config(void) {

    SetRandomSeed(time(NULL));

    SetExitKey(0);



    lognest_trace("[Raylib] Window opened.");
    SetTraceLogLevel(LOG_NONE);

    SetTargetFPS(60);

    InitAudioDevice();
}

inline void raylib_post(void) {

    lognest_trace("[Raylib] Window closed.\n");
    lognest_trace("|=====================-Wireframe Engine End-=======================|\n");
}

inline const char *bool_to_string(bool value) {
    return value ? "true" : "false";
}

inline const char *bool_to_string_c(bool value) {
    return value ? "True" : "False";
}

inline const char *enabled_disabled_cstr(bool value) {
    return value ? "Enabled" : "Disabled";
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
                  "                       ;:::::;                        \n"
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
