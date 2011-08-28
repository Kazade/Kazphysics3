#include <cassert>

#include "object.h"

#include "kp_private.h"

KP_GeomType kp_object_get_geom_type(KP_Object* obj) {
    return obj->impl->geom_type;
}

KPfloat kp_object_get_mass(KP_Object* obj) {
    return obj->impl->mass;
}

KPuint kp_object_get_user_category(KP_Object* obj) {
    return obj->impl->user_category;
}

void kp_object_get_dimensions(KP_Object* obj, KPfloat* width, KPfloat* height) {
    *width = obj->impl->geom_width;
    *height = obj->impl->geom_height;
}

void kp_object_get_position(KP_Object* obj, KPfloat* x, KPfloat* y) {
    *x = obj->impl->position.x;
    *y = obj->impl->position.y;
}

void kp_object_set_position(KP_Object* obj, KPfloat x, KPfloat y) {
    obj->impl->position.x = x;
    obj->impl->position.y = y;
}

void kp_object_set_geom_type(KP_Object* obj, KP_GeomType type) {
    obj->impl->geom_type = type;
}

void kp_object_set_mass(KP_Object* obj, KPfloat mass) {
    assert(mass >= 0 && "Can't set a negative mass");
    obj->impl->mass = mass;
}

void kp_object_set_user_category(KP_Object* obj, KPuint category) {
    obj->impl->user_category = category;
}

void kp_object_set_dimensions(KP_Object* obj, KPfloat width, KPfloat height) {
    assert(width > 0.0f && "Can't zet a negative or zero width");
    assert(height > 0.0f && "Can't zet a negative or zero height");
        
    obj->impl->geom_width = width;
    obj->impl->geom_height = height;
}

void kp_object_get_acceleration(KP_Object* obj, KPfloat* x, KPfloat* y) {
    assert(x);
    assert(y);
    assert(obj);
    
    *x = obj->impl->acceleration.x;
    *y = obj->impl->acceleration.y;
}

void kp_object_get_velocity(KP_Object* obj, KPfloat* x, KPfloat* y) {
    assert(x);
    assert(y);
    assert(obj);
    
    *x = obj->impl->velocity.x;
    *y = obj->impl->velocity.y;
}

void kp_object_set_acceleration(KP_Object* obj, KPfloat x, KPfloat y) {
    assert(obj);
    obj->impl->acceleration.x = x;
    obj->impl->acceleration.y = y;
}

void kp_object_set_velocity(KP_Object* obj, KPfloat x, KPfloat y) {
    assert(obj);
    obj->impl->velocity.x = x;
    obj->impl->velocity.y = y;
}
