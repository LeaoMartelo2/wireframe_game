#include "gui.h"

gui_color_scheme gui_transparent = {
    .default_color = BLANK,
    .hoovered_color = BLANK,
    .pressed_color = BLANK,
    .text_color = WHITE,
};

int gui_button(Rectangle *bounds, int button_style, const char *text, int font_size, const gui_color_scheme *colors) {

    int result = 0;

    static Color current_color = colors->default_color;

    Vector2 mouse_pos = GetMousePosition();

    if (CheckCollisionPointRec(mouse_pos, *bounds)) {
        current_color = colors->hoovered_color;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            current_color = colors->pressed_color;
        } else {
            current_color = colors->hoovered_color;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            result = 1;
        }

    } else {
        current_color = colors->default_color;
    }

    switch (button_style) {

    case GUI_SQUARE:
        DrawRectangleRec(*bounds, current_color);
        break;

    case GUI_ROUNDED:
        DrawRectangleRounded(*bounds, 1, 10, current_color);
        break;

    case GUI_LINES:
        DrawRectangleLinesEx(*bounds, 1, current_color);
        break;

    case GUI_LINES_ROUNDED:
        DrawRectangleRoundedLinesEx(*bounds, 0.5, 10, 0.5, current_color);
        break;
    }

    if (text != NULL) {
        int text_width = MeasureText(text, font_size);

        float text_x = bounds->x + (bounds->width / 2.0f) - (text_width / 2.0f);

        float text_y = bounds->y + bounds->height / 2.0f - font_size / 2.0f;

        DrawText(text, text_x, text_y, font_size, colors->text_color);
    }

    return result;
}

int gui_button_ex(gui_button_t *button, const char *text) {

    return gui_button(&button->bounds,
                      button->button_style,
                      text,
                      button->font_size,
                      button->colors);
}

void draw_panel(gui_panel *panel) {

    Rectangle the_panel = {
        .x = panel->pos.x,
        .y = panel->pos.y,
        .width = panel->size.x,
        .height = panel->size.y,
    };

    DrawRectangleV(panel->pos, panel->size, GetColor(0x181818FF));
    DrawRectangleLinesEx(the_panel, 5, panel->color);

    Rectangle exit_button = {
        .x = the_panel.x + panel->exit_pos.x,
        .y = the_panel.y + panel->exit_pos.y,
        .width = panel->exit_size.x,
        .height = panel->exit_size.y,
    };

    if (gui_button(&exit_button, GUI_SQUARE, "<", 100, &gui_transparent)) {
        *panel->toggle = !*panel->toggle;
    }
}

void draw_text_in_pannel_space(gui_panel *panel, const char *text, int font_size, Vector2 pos) {

    DrawText(text,
             panel->pos.x + pos.x,
             panel->pos.y + pos.y,
             font_size,
             WHITE);
}

int gui_button_on_pannel(gui_panel *panel, gui_button_t *button, const char *text) {

    Rectangle new_bounds = {
        .x = panel->pos.x + button->bounds.x,
        .y = panel->pos.y + button->bounds.y,
        .width = button->bounds.width,
        .height = button->bounds.height,
    };

    return gui_button(&new_bounds,
                      button->button_style,
                      text,
                      button->font_size,
                      button->colors);
}
