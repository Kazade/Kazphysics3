#include <unittest++/UnitTest++.h>

#include "../src/kazphysics3.h"

TEST(create_destroy) {
    KP_World* w = NULL;
    CHECK(!w);

    w = kp_world_create();
    CHECK(w);
    CHECK_EQUAL(w->id, 1);
    kp_world_destroy(&w);
    
    CHECK(!w);

    w = kp_world_create();
    CHECK(w);
    CHECK_EQUAL(w->id, 2);
    kp_world_destroy(&w);
    
    CHECK(!w);
}

TEST(step) {
    KP_World* w = kp_world_create();
    CHECK(w);
    
    kp_world_step(w, 0.1);
    CHECK_EQUAL(1, kp_world_get_step_count(w));

    kp_world_step(w, 0.1);
    CHECK_EQUAL(2, kp_world_get_step_count(w));    
    
    kp_world_step(w, 0.1);
    CHECK_EQUAL(3, kp_world_get_step_count(w));
    
    kp_world_destroy(&w);
    CHECK(!w);
}

TEST(object_creation) {
    const KPuint CHARACTER = 1;

    KPfloat mass = 1.0f;
    
    KP_World* w = kp_world_create();
    CHECK(w);
    
    CHECK_EQUAL(0, kp_world_get_object_count(w));
    
    //Create an object
    KP_Object* o = kp_world_object_create(w, KP_GEOM_ELLIPSE, mass, CHARACTER); 
    CHECK(o);
    
    CHECK_EQUAL(1, kp_world_get_object_count(w));
    
    //Take a pointer copy
    KP_Object* copy = o; 
    
    //Check the world knows about the object       
    CHECK(kp_world_object_is_valid(w, o));
    
    //Check the properties were set
    CHECK_EQUAL(KP_GEOM_ELLIPSE, kp_object_get_geom_type(o));
    CHECK_CLOSE(1.0, kp_object_get_mass(o), 0.001);
    CHECK_EQUAL(CHARACTER, kp_object_get_user_category(o));
    
    //Width and height should default to 1.0
    KPfloat width, height;
    kp_object_get_dimensions(o, &width, &height);
    CHECK_CLOSE(1.0, width, 0.001);
    CHECK_CLOSE(1.0, height, 0.001);
    
    //Position should default to the origin
    KPfloat x, y;
    kp_object_get_position(o, &x, &y);
    CHECK_CLOSE(0.0, x, 0.001);
    CHECK_CLOSE(0.0, y, 0.001);    
    
    //Destroy the object
    kp_world_object_destroy(w, &o);
    CHECK(!o);
    
    //Check the copy is no longer valid
    CHECK(!kp_world_object_is_valid(w, copy));
    //Check NULL is also invalid
    CHECK(!kp_world_object_is_valid(w, o));
    
    kp_world_destroy(&w);
}   

TEST(triangle_creation) {
    const KPuint SOLID = 1;
    
    KP_World* w = kp_world_create();
    CHECK(w);
    CHECK_EQUAL(0, kp_world_get_triangle_count(w));
        
    KP_Triangle* t = kp_world_triangle_create(w, 
                                           -100.0f, 0.0f,
                                           100.0f, 0.0f,
                                           0.0f, -5.0f,
                                           SOLID);
                                           
    CHECK_EQUAL(1, kp_world_get_triangle_count(w));
    CHECK_EQUAL(SOLID, kp_triangle_get_user_category(t));
    CHECK_CLOSE(1.0f, kp_triangle_get_friction(t), 0.001);
    
    KPfloat x, y;
    kp_triangle_get_point_at(t, 0, &x, &y);
    CHECK_CLOSE(x, -100.0f, 0.001);
    CHECK_CLOSE(y, 0.0f, 0.001);    
 
    kp_triangle_get_point_at(t, 1, &x, &y);
    CHECK_CLOSE(x, 100.0f, 0.001);
    CHECK_CLOSE(y, 0.0f, 0.001);   
    
    kp_triangle_get_point_at(t, 2, &x, &y);
    CHECK_CLOSE(x, 0.0f, 0.001);
    CHECK_CLOSE(y, -5.0f, 0.001);       
}

int main() {
    return UnitTest::RunAllTests();
}
