#include <GL/gl.h>
#include <iostream>

#include "../world.h"
#include "../triangle.h"
#include "../object.h"

#include "gl_utils.h"

void kp_world_render_in_wireframe(KP_World* world) {
    KPuint num_tris = kp_world_get_triangle_count(world);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < num_tris; ++i) {
        KPfloat x, y;
        KP_Triangle* tri = kp_world_get_triangle_at(world, i);
        
        kp_triangle_get_point_at(tri, 0, &x, &y);
        glVertex3f(x, y, 0.0f);
        
        kp_triangle_get_point_at(tri, 1, &x, &y);
        glVertex3f(x, y, 0.0f);
        
        kp_triangle_get_point_at(tri, 2, &x, &y);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
    KPuint num_objs = kp_world_get_object_count(world);
    glBegin(GL_LINES);    
    for(int i = 0; i < num_objs; ++i) {
        KP_Object* obj = kp_world_get_object_at(world, i);
        KPfloat x, y;
        KPfloat w, h;
        kp_object_get_position(obj, &x, &y);
        kp_object_get_dimensions(obj, &w, &h);
        
        //FIXME: Rotate by angle!

        glVertex3f(x - (w / 2.0f), y, 0.0f);
        glVertex3f(x + (w / 2.0f), y, 0.0f);

        glVertex3f(x, y - (h / 2.0f), 0.0f);
        glVertex3f(x, y + (h / 2.0f), 0.0f);            
    }
    glEnd();    
    glEnable(GL_TEXTURE_2D);
}
