#ifndef RENDER_RT_H_INCLUDED
#define RENDER_RT_H_INCLUDED

#include "srdefs.h"

//##############################
//###   Ray Tracer defines   ###
//##############################

typedef struct {
    vec3  normal;
    vec3  pos;
    float t;
} ray_hit_t;

//###############################
//###   Rendering functions   ###
//###############################

static void Render_RayTriangleInters( vertex_t *v0, vertex_t *v1, vertex_t *v2, SDL_Surface *image, obj_model_t *model );

static void Render_DrawLine         ( vertex_t v0, vertex_t v1, SDL_Surface *image, color_t color);
static void Render_DrawTriangle     ( vertex_t *v0, vertex_t *v1, vertex_t *v2, SDL_Surface *image, obj_model_t *model );
void Render_RT_DrawObject    ( scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface, float delta ); 

#endif // RENDER_SE_H_INCLUDED
