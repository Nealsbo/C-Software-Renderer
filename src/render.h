#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "srdefs.h"



//###########################
//###   Rendering enums   ###
//###########################

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



//#############################
//###   Rendering defines   ###
//#############################

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



<<<<<<< HEAD
=======
//###########################
//###   Scene functions   ###
//###########################

scene_t      *Scene_Init            ( camera_t *camera );
void          Scene_AddObject       ( scene_t *scene, obj_model_t *model );
List         *Scene_GetObjectList   ( scene_t *scene );
obj_model_t  *Scene_FindObjectByName( scene_t *scene, char *name );
void          Scene_PrintObjectList ( scene_t *scene );
void          Scene_Destroy         ( scene_t *scene );



>>>>>>> 2df03e5a9b8884e01c3fa6ec953dda5c4a34a6d3
//##############################
//###   Renderer functions   ###
//##############################

renderer_t *Renderer_Init ( scene_t *scene, uint32_t rs_flag, uint32_t rt_flag );

void Renderer_SwitchRendState ( renderer_t *renderer );
void Renderer_UpdateInfo      ( renderer_t *renderer ); //TODO
void Renderer_ClearZBuffer    ( renderer_t *renderer );
void Renderer_Putpixel        ( SDL_Surface *surface, int x, int y, uint32_t pixel );
<<<<<<< HEAD
void Renderer_DrawObject      ( scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface );
=======

>>>>>>> 2df03e5a9b8884e01c3fa6ec953dda5c4a34a6d3
void Renderer_DrawWorld       ( renderer_t *renderer, SDL_Surface *Surface );
void Renderer_Destroy         ( renderer_t *renderer );

#endif // RENDER_H_INCLUDED
