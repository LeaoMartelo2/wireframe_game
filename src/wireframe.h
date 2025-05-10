#ifndef WIREFRAME_RENDER_H_
#define WIREFRAME_RENDER_H_

#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#include "../raylib/rlgl.h"
#include <math.h>

// draw a 3D "thick line" by placing a thin cube between start and end
void draw_line3d_thick(Vector3 start, Vector3 end, float thickness, Color color);

void draw_wireframe(Model model, Vector3 position, float scale, Color color, float thickness);

#endif // !WIREFRAME_RENDER_H_
