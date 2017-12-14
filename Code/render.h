#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "srdefs.h"

//##########################
//###   Raster defines   ###
//##########################

typedef struct {
    float x, xStep;
    int yStart, yEnd;
} edge_t;

typedef struct {
    color_t m_color[3];
	vec4    m_colorXStep;
    vec4    m_colorYStep;
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
    bitmap_t        *z_Buffer;
    float           frameTime;
    long int        frameCount;
} renderInfo_t;

//############################
//###   Raster functions   ###
//############################

void Vertex_Init(vertex_t vert, vec3 pos, vec3 norm, vec2 uv);
void Vertex_Transform(vertex_t vert, mat4 tf);

void Edge_Init(edge_t edge, vec3 miny, vec3 maxy);
void Edge_Step(edge_t *edge);
void Edge_Print(edge_t edge);

void Gradient_Init(gradient_t grad, vec3 miny, vec3 midy, vec3 maxy);

//###########################
//###   Scene functions   ###
//###########################

scene_t *Scene_Init          ( camera_t *camera );
void    Scene_AddObject      ( scene_t *scene, obj_model_t *model );
List    *Scene_GetObjectList ( scene_t *scene );
void    Scene_PrintObjectList( scene_t *scene );
void    Scene_Destroy        ( scene_t *scene );

//###############################
//###   Rendering functions   ###
//###############################

renderContext_t *Render_InitContext(uint32_t flag);
renderInfo_t    *Render_Init(renderContext_t *context, uint32_t flag);

void Render_SwitchRenderState( renderInfo_t *render );
void Render_UpdateRenderInfo ( renderInfo_t *render ); //TODO
void Render_DrawLine         ( vec2i p0, vec2i p1, SDL_Surface *image, color_t color );
void Render_DrawTriangle     ( vec2i t0, vec2i t1, vec2i t2, SDL_Surface *image, color_t color );       //TODO: optimize, vertex, texture rendering, shading
void Render_DrawObject       ( scene_t *scene, renderInfo_t *render, obj_model_t *model, SDL_Surface *Surface, float delta );
void Render_DrawWorld        ( scene_t *scene, renderInfo_t *render, SDL_Surface *Surface, float delta );
void Render_Destroy          ( renderInfo_t *render, renderContext_t *context );
void putpixel                ( SDL_Surface *surface, int x, int y, uint32_t pixel );

#endif // RENDER_H_INCLUDED
