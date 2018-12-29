#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include "srdefs.h"

extern void Scene_Destroy( scene_t* );
extern void Renderer_Destroy( renderer_t* );

void Renderer_Putpixel(SDL_Surface *surface, int x, int y, uint32_t pixel);
static void SetupScene();
static void SceneCleanUp();

void UpdateScene( renderer_t *renderer, float delta );

void IH_ProcessInput( SDL_Event e, renderer_t *renderer );
void IH_Handle( SDL_Event e, renderer_t *renderer );

int  Application_Init ();
void Application_Run  ();
void Application_Close();

#endif
