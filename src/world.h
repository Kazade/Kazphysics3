#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

extern "C" {

#include "types.h"

struct KP_WorldImpl;

typedef struct KP_World {
    KP_WorldImpl* impl;
    KPuint id;
} KP_World;

struct KP_Object;
struct KP_Triangle;

KP_World* kp_world_create();
void kp_world_destroy(KP_World** world);
void kp_world_step(KP_World* world, float dt);

KPuint kp_world_get_step_count(KP_World* world);

KP_Object* kp_world_object_create(KP_World* world, KP_GeomType geom, KPfloat mass, KPuint user_category);
void kp_world_object_destroy(KP_World* world, KP_Object** object);
bool kp_world_object_is_valid(KP_World* world, KP_Object* object);

KP_Triangle* kp_world_triangle_create(KP_World* world, KPfloat x1, KPfloat y1,
                                                       KPfloat x2, KPfloat y2,
                                                       KPfloat x3, KPfloat y3,
                                                       KPuint user_category);
KPuint kp_world_get_triangle_count(KP_World* world);                                                       
KP_Triangle* kp_world_get_triangle_at(KP_World* world, KPuint idx);

KPuint kp_world_get_object_count(KP_World* world);
KP_Object* kp_world_get_object_at(KP_World* world, KPuint idx);

}

/**
    USAGE:
        bool prepare_collision_callback(KP_World* world, KP_Object* this, void* data) {
            //A false return will not run the collision process on this entity, this step
            //Called before this object's collisions are processed, won't be called however
            //if this object is collided by another object.
            // e.g called before this.collides(other), but not other.collides(this)

            if(character_is_dead()) {
                return false;
            }
            return true;
        }

        void post_collision_callback(KP_World* world, KPObject* this, void* data) {
            //Called after all collisions are processed
        }

        void collision_callback_obj(KP_World* world, KP_Object* this, KP_Object* other, KP_Collision* info, void* data) {
            if(kp_object_get_user_type(other) == RING) {
                kp_world_object_destroy(world, other);
                increase_ring_count();
            }
        }

        void collision_callback_tri(KP_World* world, KP_Object* this, KP_Triangle* other, KP_Collision* info, void* data) {
            if(kp_triangle_get_user_type(other) == BREAKABLE &&
               character_is_rolling()) {
                kp_world_triangle_destroy(world, other);
                increase_score(100);
            }

            kmVec3 normal;
            kp_collision_get_other_normal(&normal);
            if(kmVec3DotProduct(normal, x_axis) > 0) { //Erm, whatever
                set_character_on_ground(true);
            }
        }

        KP_World* w = kp_world_create();

        kp_world_set_gravity(x, y);
        kp_world_triangle_create(w, x1, y1, z1,
                                 x2, y2, z2,
                                 x3, y3, z3,
                                 friction,
                                 user_type);

        KPfloat width = 5.0f;
        KPfloat height = 10.0f;
        KPfloat mass = 1.0f;
        KPint type_id = CHARACTER;

        KP_Object* o = kp_world_object_create(w, KP_GEOM_ELLIPSE, mass, type_id);
        kp_object_set_dimensions(width, height);
        kp_object_set_bounciness(5.0f);
        kp_object_set_friction(0.1f);
        kp_object_set_rotation_degrees(45.0f);
        kp_object_set_velocity_local(5.0f, 0.0f); // Sets the velocity depending on rotation
        kp_object_set_user_data(character);

        void* data = NULL;
        kp_object_set_collision_prepare_cb(prepare_collision_cb, data);
        kp_object_set_collision_cb_obj(collision_callback_obj, data);
        kp_object_set_collision_cb_tri(collision_callback_tri, data);

        while(running) {
            kp_world_step(w, dt);
        }

        kp_world_destroy(&w);
*/
#endif // WORLD_H_INCLUDED
