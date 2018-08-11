#ifndef RENDER_SE_H_INCLUDED
#define RENDER_SE_H_INCLUDED

#include "srdefs.h"



//##########################
//###   Raster defines   ###
//##########################

typedef struct {
    float x;
    float xStep;
    int   yStart;
    int   yEnd;
    float texCoordX;
    float texCoordXStep;
    float texCoordY;
    float texCoordYStep;
    float oneOverZ;
    float oneOverZStep;
    float depth;
    float depthStep;
} edge_t;

typedef struct {
    float texCoordX[3];
    float texCoordY[3];
    float oneOverZ[3];
    float depth[3];

    float texCoordXXStep;
    float texCoordXYStep;
    float texCoordYXStep;
    float texCoordYYStep;
    float oneOverZXStep;
    float oneOverZYStep;
    float depthXStep;
    float depthYStep;
} gradient_t;



//############################
//###   Raster functions   ###
//############################

edge_t     Edge_Init ( gradient_t grad, vertex_t *miny, vertex_t *maxy, int miny_index );
void       Edge_Step ( edge_t *edge );
void       Edge_Print( edge_t edge );

gradient_t Gradient_Init( vertex_t *miny, vertex_t *midy, vertex_t *maxy );
float      Gradient_CalcXStep( float *values, vertex_t *miny, vertex_t *midy, vertex_t *maxy, float oneOverdX );
float      Gradient_CalcYStep( float *values, vertex_t *miny, vertex_t *midy, vertex_t *maxy, float oneOverdX );



//###############################
//###   Rendering functions   ###
//###############################

static void Render_DrawLine         ( vertex_t v0, vertex_t v1, SDL_Surface *image, color_t color);
static void Render_DrawTriangle     ( vertex_t *v0, vertex_t *v1, vertex_t *v2, SDL_Surface *image, obj_model_t *model );
void Render_SE_DrawObject       ( scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface, float delta );

#endif // RENDER_SE_H_INCLUDED
