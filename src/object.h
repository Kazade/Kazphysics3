#ifndef KP_OBJECT_H
#define KP_OBJECT_H

#include "types.h"

extern "C" {

struct KP_ObjectImpl;

typedef struct KP_Object {
    KP_ObjectImpl* impl;
} KP_Object;

KP_GeomType kp_object_get_geom_type(KP_Object* obj);
KPfloat kp_object_get_mass(KP_Object* obj);
KPuint kp_object_get_user_category(KP_Object* obj);
void kp_object_get_dimensions(KP_Object* obj, KPfloat* width, KPfloat* height);
void kp_object_get_position(KP_Object* obj, KPfloat* x, KPfloat* y);
void kp_object_get_acceleration(KP_Object* obj, KPfloat* x, KPfloat* y);
void kp_object_get_velocity(KP_Object* obj, KPfloat* x, KPfloat* y);

void kp_object_set_mass(KP_Object* obj, KPfloat mass);
void kp_object_set_user_category(KP_Object* obj, KPuint category);
void kp_object_set_dimensions(KP_Object* obj, KPfloat width, KPfloat height);
void kp_object_set_position(KP_Object* obj, KPfloat x, KPfloat y);
void kp_object_set_velocity(KP_Object* obj, KPfloat x, KPfloat y);
void kp_object_set_acceleration(KP_Object* obj, KPfloat x, KPfloat y);

}

#endif
