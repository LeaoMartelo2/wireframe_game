#ifndef GUI_H_
#define GUI_H_

#include "../raylib/raylib.h"
#include <stdio.h>

typedef enum {
    GUI_SQUARE = 0,
    GUI_ROUNDED,
    GUI_LINES,
    GUI_LINES_ROUNDED,
} button_style;

typedef struct {
    Color default_color;
    Color hoovered_color;
    Color pressed_color;
    Color text_color;
} gui_color_scheme;

typedef struct {
    Rectangle bounds;
    int button_style;
    int font_size;
    const gui_color_scheme *colors;
} gui_button_t;

// modified implementation of RayGui buttons
// https://github.com/raysan5/raygui
int gui_button(const Rectangle *bounds, const int button_style, const char *text, const int font_size, const gui_color_scheme *colors);

int gui_button_ex(const gui_button_t *button, const char *text);

#endif // !GUI_H_
