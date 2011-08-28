#ifndef KP_PRIVATE_H
#define KP_PRIVATE_H

#include <vector>
#include "types.h"

#include "kazmath/vec2.h"

struct KP_Object;
struct KP_Triangle;
struct KP_World;

typedef struct KP_WorldImpl {
    KPuint step_count;
    
    kmVec2 gravity;
    
    std::vector<KP_Object*> objects;
    std::vector<KP_Triangle*> triangles;
    
} KP_WorldImpl;

/* Private world methods */

void kp_world_store_object(KP_World* world, KP_Object* obj);
void kp_world_unstore_object(KP_World* world, KP_Object* obj);
bool kp_world_contains_object(KP_World* world, KP_Object* obj);

void kp_world_store_triangle(KP_World* world, KP_Triangle* tri);
void kp_world_unstore_triangle(KP_World* world, KP_Triangle* tri);
bool kp_world_contains_triangle(KP_World* world, KP_Triangle* tri);

typedef struct KP_ObjectImpl {
    KP_GeomType geom_type;
    KPfloat geom_width;
    KPfloat geom_height;
    
    kmVec2 position;
    kmVec2 velocity;
    kmVec2 acceleration;
    
    KPfloat mass;
    KPuint user_category;
    
} KP_ObjectImpl;


/* Private object methods */
void kp_object_set_geom_type(KP_Object* obj, KP_GeomType type);
    
typedef struct KP_TriangleImpl {
    kmVec2 points[3];
    
    KPfloat friction;
    KPuint user_category;
} KP_TriangleImpl;

/* Private triangle methods */
void kp_triangle_set_point(KP_Triangle* tri, KPuint which, KPfloat x, KPfloat y);

#endif
