#include <cassert>

#include "triangle.h"
#include "kp_private.h"

void kp_triangle_set_friction(KP_Triangle* tri, KPfloat friction) {
    assert(tri);
    
    tri->impl->friction = friction;
}

void kp_triangle_set_user_category(KP_Triangle* tri, KPuint user_category) {
    assert(tri);
    
    tri->impl->user_category = user_category;
}

KPfloat kp_triangle_get_friction(KP_Triangle* tri) {
    assert(tri);
    
    return tri->impl->friction;
}

KPuint kp_triangle_get_user_category(KP_Triangle* tri) {
    assert(tri);
    
    return tri->impl->user_category;
}

void kp_triangle_get_point_at(KP_Triangle* tri, KPuint p, KPfloat* x, KPfloat* y) {
    assert(p >= 0 && p < 3);
    assert(tri);
    
    *x = tri->impl->points[p].x;
    *y = tri->impl->points[p].y;
}

void kp_triangle_set_point(KP_Triangle* tri, KPuint which, KPfloat x, KPfloat y) {
    assert(tri);
    assert(which >= 0 && which < 3);
    
    tri->impl->points[which].x = x;
    tri->impl->points[which].y = y;
}
