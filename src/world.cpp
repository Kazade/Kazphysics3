#include <cstdlib>
#include <new>
#include <algorithm>
#include <limits>

#include "kazmath/aabb.h"
#include "world.h"
#include "object.h"
#include "triangle.h"

#include "kp_private.h"

static KPuint counter = 0;

static void init_impl(KP_WorldImpl* impl) {
    impl->step_count = 0;
    impl->gravity.x = 0.0f;
    impl->gravity.y = -9.8f;
}

KP_World* kp_world_create() {
    KP_World* result = new KP_World();
    result->impl = new KP_WorldImpl();

    init_impl(result->impl);
    
    result->id = ++counter;
    return result;
}

void kp_world_destroy(KP_World** world) {
    delete (*world)->impl;
    delete *world;
    *world = NULL;
}

/**
    Given a source position, dest position and object dimensions. This returns an AABB
    that contains all potential colliders for the object.
*/
void kp_get_collidable_test_box(const kmVec2* source_pos, const kmVec2* dest_pos, const kmVec2* dimensions, kmAABB* out) {
    kmAABB source_box, dest_box, final_box;
    source_box.min.x = source_pos->x - (dimensions->x * 0.5);
    source_box.min.y = source_pos->y - (dimensions->y * 0.5);
    source_box.min.z = -std::numeric_limits<float>::max(); //We don't care about Z just make it the min possible

    source_box.max.x = source_pos->x + (dimensions->x * 0.5);
    source_box.max.y = source_pos->y + (dimensions->y * 0.5);
    source_box.max.z = std::numeric_limits<float>::max(); //We don't care about Z just make it the max possible


    dest_box.min.x = dest_pos->x - (dimensions->x * 0.5);
    dest_box.min.y = dest_pos->y - (dimensions->y * 0.5);
    dest_box.min.z = -std::numeric_limits<float>::max(); //We don't care about Z just make it the min possible

    dest_box.max.x = dest_pos->x + (dimensions->x * 0.5);
    dest_box.max.y = dest_pos->y + (dimensions->y * 0.5);
    dest_box.max.z = std::numeric_limits<float>::max(); //We don't care about Z just make it the max possible
    
    out->min.x = std::min(source_box.min.x, dest_box.min.x);
    out->min.y = std::min(source_box.min.y, dest_box.min.y);    
    out->min.z = std::min(source_box.min.z, dest_box.min.z);        
    
    out->max.x = std::max(source_box.max.x, dest_box.max.x);
    out->max.y = std::max(source_box.max.y, dest_box.max.y);    
    out->max.z = std::max(source_box.max.z, dest_box.max.z);            
}

std::vector<KP_Triangle*> kp_world_find_potential_collider_triangles(KP_World* world, KP_Object* obj, const kmVec2* dest) {
    //Get the source position
    kmVec2 source;
    kp_object_get_position(obj, &source.x, &source.y);

    /*
        FIXME: This will not work if the object is rotated!!!
    */
    kmVec2 dim;
    kp_object_get_dimensions(obj, &dim.x, &dim.y);

    //Get the bounding box of the source and destination
    kmAABB container;
    kp_get_collidable_test_box(&source, dest, &dim, &container);
    
    std::vector<KP_Triangle*> result;
    
    /*
        FIXME: Use some kind of spacial heirarchy to speed this up!
    */
    for(int i = 0; i < kp_world_get_triangle_count(world); ++i) {
        KP_Triangle* tri = kp_world_get_triangle_at(world, i);
        
        bool intersects = true; //FIXME: Err... obviously
        /*bool intersects = kmAABBIntersectsTriangle(&container, 
            &tri->impl->points[0],
            &tri->impl->points[1],
            &tri->impl->points[2]
        );*/
        
        if(intersects) {
            result.push_back(tri);
        }
    }
    
    return result;
}

void kp_world_attempt_object_move(KP_World* world, KP_Object* obj, const kmVec2* vel, float dt) {
    kmVec2 pos;
    kp_object_get_position(obj, &pos.x, &pos.y);
    
    pos.x += vel->x * dt;
    pos.y += vel->y * dt;
    
    kp_object_set_position(obj, pos.x, pos.y);
}

void kp_world_step(KP_World* world, float dt) {
    for(KPuint i = 0; i < kp_world_get_object_count(world); ++i) {
        KP_Object* obj = kp_world_get_object_at(world, i);
        
        //Increase the velocity by the acceleration
        kmVec2 acc, vel;
        
        kp_object_get_acceleration(obj, &acc.x, &acc.y);
        kp_object_get_velocity(obj, &vel.x, &vel.y);
        
        //Scale the acceleration by the dt value
        kmVec2Scale(&acc, &acc, dt);
        
        kmVec2 new_vel;
        //Add the acceleration to the velocity
        kmVec2Add(&new_vel, &acc, &vel);
        kp_object_set_velocity(obj, new_vel.x, new_vel.y);
        
        //If the world has gravity set
        if(kmVec2Length(&world->impl->gravity) > 0.001) {    
            //Attempt to move the object using the gravity as the velocity
            kp_world_attempt_object_move(world, obj, &world->impl->gravity, dt);
        }
        
        //Now, attempt to move the object by its current velocity
        kp_world_attempt_object_move(world, obj, &obj->impl->velocity, dt);
    }

    world->impl->step_count++;
}

KPuint kp_world_get_step_count(KP_World* world) {
    return world->impl->step_count;
}

KP_Object* kp_world_object_create(KP_World* world, KP_GeomType geom, KPfloat mass, KPuint user_category) {
    KP_Object* obj = new KP_Object();
    obj->impl = new KP_ObjectImpl();
    
    kp_object_set_geom_type(obj, geom);
    kp_object_set_mass(obj, mass);
    kp_object_set_user_category(obj, user_category);
    kp_object_set_dimensions(obj, 1.0f, 1.0f);
    kp_object_set_acceleration(obj, 0.0f, 0.0f);
    kp_object_set_velocity(obj, 0.0f, 0.0f);
    
    kp_world_store_object(world, obj);
    
    return obj;
}

void kp_world_object_destroy(KP_World* world, KP_Object** object) {
    kp_world_unstore_object(world, *object);

    delete (*object)->impl;
    delete (*object);
    *object = NULL;
}

bool kp_world_object_is_valid(KP_World* world, KP_Object* object) {
    return kp_world_contains_object(world, object);
}

KP_Triangle* kp_world_triangle_create(KP_World* world, KPfloat x1, KPfloat y1,
                                                       KPfloat x2, KPfloat y2,
                                                       KPfloat x3, KPfloat y3,
                                                       KPuint user_category) {
    KP_Triangle* tri = new KP_Triangle();
    tri->impl = new KP_TriangleImpl();
    
    kp_triangle_set_point(tri, 0, x1, y1);
    kp_triangle_set_point(tri, 1, x2, y2);
    kp_triangle_set_point(tri, 2, x3, y3);        
    kp_triangle_set_friction(tri, 1.0f);
    kp_triangle_set_user_category(tri, user_category);
    
    kp_world_store_triangle(world, tri);
    
    return tri;
}

KPuint kp_world_get_triangle_count(KP_World* world) {
    return world->impl->triangles.size();
}


/* =============== PRIVATE METHODS ================= */

void kp_world_store_object(KP_World* world, KP_Object* obj) {
    world->impl->objects.push_back(obj);
}

void kp_world_unstore_object(KP_World* world, KP_Object* obj) {
    std::vector<KP_Object*>& objs = world->impl->objects;
    objs.erase(std::remove(objs.begin(), objs.end(), obj), objs.end());
}

bool kp_world_contains_object(KP_World* world, KP_Object* obj) {
    std::vector<KP_Object*>& objs = world->impl->objects;
    return std::find(objs.begin(), objs.end(), obj) != objs.end();
}

void kp_world_store_triangle(KP_World* world, KP_Triangle* tri) {
    world->impl->triangles.push_back(tri);
}

void kp_world_unstore_triangle(KP_World* world, KP_Triangle* tri) {
    std::vector<KP_Triangle*>& tris = world->impl->triangles;
    tris.erase(std::remove(tris.begin(), tris.end(), tri), tris.end());
}

bool kp_world_contains_triangle(KP_World* world, KP_Triangle* tri) {
    return std::find(world->impl->triangles.begin(),
                     world->impl->triangles.end(),
                     tri) != world->impl->triangles.end();
}

KP_Triangle* kp_world_get_triangle_at(KP_World* world, KPuint idx) {
    return world->impl->triangles.at(idx);
}

KPuint kp_world_get_object_count(KP_World* world) {
    return world->impl->objects.size();
}

KP_Object* kp_world_get_object_at(KP_World* world, KPuint i) {
    return world->impl->objects.at(i);
}


