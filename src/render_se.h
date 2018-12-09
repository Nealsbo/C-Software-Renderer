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
    float light;
    float lightStep;
} edge_t;

typedef struct {
    float texCoordX[3];
    float texCoordY[3];
    float oneOverZ[3];
    float depth[3];
    float light[3];

    float texCoordXXStep;
    float texCoordXYStep;
    float texCoordYXStep;
    float texCoordYYStep;
    float oneOverZXStep;
    float oneOverZYStep;
    float depthXStep;
    float depthYStep;
    float lightXStep;
    float lightYStep;
} gradient_t;



//############################
//###   Raster functions   ###
//############################

static edge_t     Edge_Init ( gradient_t grad, vertex_t *miny, vertex_t *maxy, int miny_index );
static void       Edge_Step ( edge_t *edge );
static void       Edge_Print( edge_t edge );

static gradient_t Gradient_Init( vertex_t *miny, vertex_t *midy, vertex_t *maxy );
static float      Gradient_CalcXStep( float *values, vertex_t *miny, vertex_t *midy, vertex_t *maxy, float oneOverdX );
static float      Gradient_CalcYStep( float *values, vertex_t *miny, vertex_t *midy, vertex_t *maxy, float oneOverdX );



//###############################
//###   Rendering functions   ###
//###############################

static void Render_DrawLine         ( vertex_t v0, vertex_t v1, SDL_Surface *image, color_t color);
static void Render_DrawTriangle     ( vertex_t *v, SDL_Surface *image, obj_model_t *model );
void Render_SE_DrawObject       ( scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface, float delta );

#endif // RENDER_SE_H_INCLUDED
