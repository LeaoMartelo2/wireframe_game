#include "wireframe.h"

void draw_line3d_thick(Vector3 start, Vector3 end, float thickness, Color color) {

    Vector3 delta = Vector3Subtract(end, start);
    float lenght = Vector3Length(delta);

    if (lenght <= 0.0f) {
        // turn down for what
        return;
    }

    Vector3 mid = Vector3Add(start, Vector3Scale(delta, 0.5f));

    Vector3 dir = Vector3Scale(delta, 1 / lenght);

    Vector3 up = {0, 0, 1};

    Vector3 axis = Vector3CrossProduct(up, dir);
    float axis_lenght = Vector3Length(axis);

    float angle = 0.0f;

    if (axis_lenght > 0.0001f) {

        axis = Vector3Scale(axis, 1 / axis_lenght);
        float d = Vector3DotProduct(up, dir);

        angle = acosf(d) * RAD2DEG;
    }

    rlPushMatrix();
    {

        rlTranslatef(mid.x, mid.y, mid.z);

        if (axis_lenght > 0.0001f) {
            rlRotatef(angle, axis.x, axis.y, axis.z);
        }

        DrawCube({0, 0, 0}, thickness, thickness, lenght, color);
    }
    rlPopMatrix();
}

void draw_wireframe(Model model, Vector3 position, float scale, Color color, float thickness) {

    for (int m = 0; m < model.meshCount; m++) {

        Mesh mesh = model.meshes[m];
        float *verts = mesh.vertices;
        unsigned short *inds = mesh.indices;
        int triangle_count = mesh.triangleCount;

        for (int i = 0; i < triangle_count * 3; i += 3) {

            int i0 = inds[i + 0];
            int i1 = inds[i + 1];
            int i2 = inds[i + 2];

            Vector3 v0 = {verts[3 * i0 + 0] * scale + position.x,
                          verts[3 * i0 + 1] * scale + position.y,
                          verts[3 * i0 + 2] * scale + position.z};

            Vector3 v1 = {verts[3 * i1 + 0] * scale + position.x,
                          verts[3 * i1 + 1] * scale + position.y,
                          verts[3 * i1 + 2] * scale + position.z};

            Vector3 v2 = {verts[3 * i2 + 0] * scale + position.x,
                          verts[3 * i2 + 1] * scale + position.y,
                          verts[3 * i2 + 2] * scale + position.z};

            draw_line3d_thick(v0, v1, thickness, color);
            draw_line3d_thick(v1, v2, thickness, color);
            draw_line3d_thick(v2, v0, thickness, color);
        }
    }
}

void draw_wireframe_ex(Model model, Vector3 position, Vector3 rotation_axis, float rotation_angle, Vector3 scale, Color tint, float thickness) {

    for (int m = 0; m < model.meshCount; m++) {

        Mesh mesh = model.meshes[m];

        float *verts = mesh.vertices;
        unsigned short *inds = mesh.indices;
        int triangle_count = mesh.triangleCount;

        for (int i = 0; i < triangle_count * 3; i += 3) {

            int i0 = inds[i + 0];
            int i1 = inds[i + 1];
            int i2 = inds[i + 2];

            Vector3 v0_local = {verts[3 * i0 + 0], verts[3 * i0 + 1], verts[3 * i0 + 2]};
            Vector3 v1_local = {verts[3 * i1 + 0], verts[3 * i1 + 1], verts[3 * i1 + 2]};
            Vector3 v2_local = {verts[3 * i2 + 0], verts[3 * i2 + 1], verts[3 * i2 + 2]};

            Vector3 v0_scaled = {v0_local.x * scale.x, v0_local.y * scale.y, v0_local.z * scale.z};
            Vector3 v1_scaled = {v1_local.x * scale.x, v1_local.y * scale.y, v1_local.z * scale.z};
            Vector3 v2_scaled = {v2_local.x * scale.x, v2_local.y * scale.y, v2_local.z * scale.z};

            Vector3 v0_rot = Vector3RotateByAxisAngle(v0_scaled, rotation_axis, rotation_angle * DEG2RAD);
            Vector3 v1_rot = Vector3RotateByAxisAngle(v1_scaled, rotation_axis, rotation_angle * DEG2RAD);
            Vector3 v2_rot = Vector3RotateByAxisAngle(v2_scaled, rotation_axis, rotation_angle * DEG2RAD);

            Vector3 v0 = Vector3Add(v0_rot, position);
            Vector3 v1 = Vector3Add(v1_rot, position);
            Vector3 v2 = Vector3Add(v2_rot, position);

            draw_line3d_thick(v0, v1, thickness, tint);
            draw_line3d_thick(v1, v2, thickness, tint);
            draw_line3d_thick(v2, v0, thickness, tint);
        }
    }
}
