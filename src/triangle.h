#ifndef KP_TRIANGLE_H
#define KP_TRIANGLE_H

#include "types.h"

extern "C" {

struct KP_TriangleImpl;

typedef struct KP_Triangle {
    KP_TriangleImpl* impl;
} KP_Triangle;

void kp_triangle_set_friction(KP_Triangle* tri, KPfloat friction);
void kp_triangle_set_user_category(KP_Triangle* tri, KPuint user_category);
KPfloat kp_triangle_get_friction(KP_Triangle* tri);
KPuint kp_triangle_get_user_category(KP_Triangle* tri);
void kp_triangle_get_point_at(KP_Triangle* tri, KPuint p, KPfloat* x, KPfloat* y);

}

#endif
