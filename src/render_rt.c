#include "srdefs.h"
#include "render_rt.h"

extern renderer_t *renderer_;

static void Render_RayTriangleInters( vertex_t *v0, vertex_t *v1, vertex_t *v2, SDL_Surface *image, obj_model_t *model ){
    
}

static void Render_DrawLine(vertex_t v0, vertex_t v1, SDL_Surface *image, color_t color) {

}

static void Render_DrawTriangle( vertex_t *miny, vertex_t *midy, vertex_t *maxy, SDL_Surface *Surface, obj_model_t *model ){

}

void Render_RT_DrawObject(scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface, float delta){

} 
