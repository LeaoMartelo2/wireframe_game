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

extern gui_color_scheme gui_transparent;

// modified implementation of RayGui buttons
// https://github.com/raysan5/raygui
int gui_button(Rectangle *bounds, int button_style, const char *text, int font_size, const gui_color_scheme *colors);

int gui_button_ex(gui_button_t *button, const char *text);

typedef struct {
    Vector2 pos;
    Vector2 size;
    Color color;
    bool *toggle;
    Vector2 exit_pos;
    Vector2 exit_size;
} gui_panel;

void draw_panel(gui_panel *panel);

void draw_text_in_pannel_space(gui_panel *panel, const char *text, int font_size, Vector2 pos);

int gui_button_on_pannel(gui_panel *panel, gui_button_t *button, const char *text);

#endif // !GUI_H_
