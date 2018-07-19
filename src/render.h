#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

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

//###########################
//###   Rendering enums   ###
//###########################

enum SceneState_e{
    SCENE_STATE_FREEZE = 0,
    SCENE_STATE_RENDER = 1,
    SCENE_STATE_UPDATE_SCENE = 2
};

enum RenderState_e{
    RENDER_STATE_WIREFRAME = 0,
    RENDER_STATE_LIT = 1,
    RENDER_STATE_UNLIT = 2,
    RENDER_STATE_Z_BUFFER = 3
};

//###################################
//###   Scene/Rendering defines   ###
//###################################

typedef struct {
    obj_model_t       *skybox;
    camera_t          *mainCamera;
    List              *objectList;
    enum SceneState_e currentState;
} scene_t;

typedef struct {
    uint32_t flagState;
    int      fog;
    float    fogIntence;
} renderContext_t;

typedef struct {
    renderContext_t *context;
    float           *z_Buffer;
    float           frameTime;
    uint32_t        frameCount;
} renderInfo_t;

//############################
//###   Raster functions   ###
//############################

edge_t     Edge_Init ( gradient_t grad, vertex_t *miny, vertex_t *maxy, int miny_index );
void       Edge_Step ( edge_t *edge );
void       Edge_Print( edge_t edge );

gradient_t Gradient_Init( vertex_t *miny, vertex_t *midy, vertex_t *maxy );
float      Gradient_CalcXStep( float *values, vertex_t *miny, vertex_t *midy, vertex_t *maxy, float oneOverdX );
float      Gradient_CalcYStep( float *values, vertex_t *miny, vertex_t *midy, vertex_t *maxy, float oneOverdX );

//###########################
//###   Scene functions   ###
//###########################

scene_t *Scene_Init           ( camera_t *camera );
void     Scene_AddObject      ( scene_t *scene, obj_model_t *model );
List    *Scene_GetObjectList  ( scene_t *scene );
void     Scene_PrintObjectList( scene_t *scene );
void     Scene_Destroy        ( scene_t *scene );

//###############################
//###   Rendering functions   ###
//###############################

renderContext_t *Render_InitContext( uint32_t flag );
renderInfo_t    *Render_Init( renderContext_t *context, uint32_t flag );

void Render_SwitchRenderState( renderInfo_t *render );
void Render_UpdateRenderInfo ( renderInfo_t *render ); //TODO
void Render_ClearZBuffer     ( renderInfo_t *render );
void Render_DrawLine         ( vertex_t v0, vertex_t v1, SDL_Surface *image, color_t color);
void Render_DrawTriangle     ( vertex_t *v0, vertex_t *v1, vertex_t *v2, SDL_Surface *image, obj_model_t *model );
void Render_DrawObject       ( scene_t *scene, renderInfo_t *render, obj_model_t *model, SDL_Surface *Surface, float delta );
void Render_DrawWorld        ( scene_t *scene, renderInfo_t *render, SDL_Surface *Surface, float delta );
void Render_Destroy          ( renderInfo_t *render, renderContext_t *context );
void putpixel                ( SDL_Surface *surface, int x, int y, uint32_t pixel );

#endif // RENDER_H_INCLUDED
