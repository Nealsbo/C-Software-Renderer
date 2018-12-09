#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "srdefs.h"



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

enum RenderType_e{
    RENDER_TYPE_SOFTWARE_EDGE = 0,
    RENDER_TYPE_SOFTWARE_BARYCENTRIC = 1,
    RENDER_TYPE_RAYTRACING = 2
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
	scene_t         *scene;
    uint32_t        flagState;
    uint32_t        renderType;
    int             fog;
    float           fogIntence;
    float           *z_Buffer;
    float           frameTime;
    uint32_t        frameCount;
    void            (*PutPixel)(SDL_Surface *surface, int x, int y, uint32_t pixel);
} renderer_t;



//###########################
//###   Scene functions   ###
//###########################

scene_t *Scene_Init           ( camera_t *camera );
void     Scene_AddObject      ( scene_t *scene, obj_model_t *model );
List    *Scene_GetObjectList  ( scene_t *scene );
void     Scene_PrintObjectList( scene_t *scene );
void     Scene_Destroy        ( scene_t *scene );



//##############################
//###   Renderer functions   ###
//##############################

renderer_t *Renderer_Init ( scene_t *scene, uint32_t rs_flag, uint32_t rt_flag );

void Renderer_SwitchRendState ( renderer_t *renderer );
void Renderer_UpdateInfo      ( renderer_t *renderer ); //TODO
void Renderer_ClearZBuffer    ( renderer_t *renderer );
void Renderer_Putpixel        ( SDL_Surface *surface, int x, int y, uint32_t pixel );

void Renderer_DrawWorld       ( scene_t *scene, renderer_t *renderer, SDL_Surface *Surface, float delta );
void Renderer_Destroy         ( renderer_t *renderer );

#endif // RENDER_H_INCLUDED
