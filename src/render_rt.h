#ifndef RENDER_RT_H_INCLUDED
#define RENDER_RT_H_INCLUDED

#include "srdefs.h"

//##############################
//###   Ray Tracer defines   ###
//##############################

typedef struct {
    vec3  normal;
    vec3  pos;
    float t, u, v;
} ray_hit_t;

//###############################
//###   Rendering functions   ###
//###############################

static bool Render_RayTriangleInters( ray *r, vertex_t *v0, vertex_t *v1, vertex_t *v2, ray_hit_t *hit, SDL_Surface *image, obj_model_t *model  );

static void Render_DrawTriangle     ( vertex_t *v0, vertex_t *v1, vertex_t *v2, SDL_Surface *image, obj_model_t *model );
void Render_RT_DrawObject    ( scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface, float delta ); 

#endif // RENDER_SE_H_INCLUDED
