#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "kaztimer/kaztimer.h"

#include "../src/kazphysics3.h"
#include "../src/utils/gl_utils.h"

int create_window() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    int w = 640;
    int h = 480;

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);
    SDL_Surface* screen = SDL_SetVideoMode(w, h, 16, SDL_OPENGL);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();						// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)w/(GLfloat)h, 0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
    glLoadIdentity();							// Reset The Modelview Matrix

    return 0;
}

void create_world(KP_World* w) {
    kp_world_triangle_create(w, -100.0f, 0.0f, 100.0f, 0.0f, 0.0f, -5.0f, 0);
    
    KP_Object* obj = kp_world_object_create(w, KP_GEOM_ELLIPSE, 1.0f, 0);
    kp_object_set_position(obj, 0.0f, 1.0f);
    kp_object_set_dimensions(obj, 0.5, 1.0f);
}

int main(int argc, char* argv[]) {
    if(create_window()) {
        return 1;
    }

    KTIuint timer;
    ktiGenTimers(1, &timer);
    ktiBindTimer(timer);
    ktiStartFixedStepTimer(45);
    
    KP_World* w = kp_world_create();
    create_world(w);
    
    bool running = true;
    while(running) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                break;
                case SDL_KEYDOWN:
                break;
                case SDL_KEYUP:
                break;
                default:
                break;
            }
        }

        ktiUpdateFrameTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -15.0f);
        
        while(ktiTimerCanUpdate()) {
            kp_world_step(w, ktiGetDeltaTime());
        }
        
        kp_world_render_in_wireframe(w);
        
        SDL_GL_SwapBuffers();
    }
    return 0;
}

