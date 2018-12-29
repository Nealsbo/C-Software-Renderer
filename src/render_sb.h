#ifndef RENDER_SR_H_INCLUDED
#define RENDER_SR_H_INCLUDED

#include "srdefs.h"

//############################
//###   Raster functions   ###
//############################

static vec3 barycentric(vec2 *pts, vec2i p);

//###############################
//###   Rendering functions   ###
//###############################

static void Render_DrawLine         ( vertex_t v0, vertex_t v1, SDL_Surface *image, color_t color);
static void Render_DrawTriangle     ( vertex_t *v, SDL_Surface *image, obj_model_t *model );
void Render_SE_DrawObject       ( scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface, float delta );

#endif // RENDER_SE_H_INCLUDED
