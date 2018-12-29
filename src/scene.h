#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include "srdefs.h"

enum SceneState_e{
    SCENE_STATE_FREEZE = 0,
    SCENE_STATE_RENDER = 1,
    SCENE_STATE_UPDATE_SCENE = 2
};

//#########################
//###   Scene defines   ###
//#########################

typedef struct {
    obj_model_t       *skybox;
    camera_t          *mainCamera;
    vec4              dummLight;
    List              *objectList;
    enum SceneState_e currentState;
} scene_t;



//###########################
//###   Scene functions   ###
//###########################

scene_t     *Scene_Init            ( camera_t *camera );
void         Scene_AddObject       ( scene_t *scene, obj_model_t *model );
List        *Scene_GetObjectList   ( scene_t *scene );
void         Scene_PrintObjectList ( scene_t *scene );
obj_model_t *Scene_FindObjectByName( scene_t *scene, char *name );
void         Scene_Destroy         ( scene_t *scene );

#endif // SCENE_H_INCLUDED
