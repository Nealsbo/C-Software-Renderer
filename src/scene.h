#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include "srdefs.h"

enum SceneState_e {
    SCENE_STATE_FROZEN    = 0,
    SCENE_STATE_UPDATABLE = 1
};

//#########################
//###   Scene defines   ###
//#########################

typedef struct {
    camera_t          *mainCamera;
    vec3               directLight;
    vec3               directLightColor;
    vec3               ambientLightColor;
    List              *objectList;
    enum SceneState_e  currentState;
} scene_t;



//###########################
//###   Scene functions   ###
//###########################

scene_t     *Scene_Init            ( camera_t *camera );
void         Scene_LoadScene       ( scene_t *scene, const char *scene_name );
void         Scene_AddObject       ( scene_t *scene, obj_model_t *model );
void         Scene_SetDirectLight  ( scene_t *scene, vec3 dir, vec3 color );
void         Scene_SetAmbientLight ( scene_t *scene, vec3 color );
List        *Scene_GetObjectList   ( scene_t *scene );
void         Scene_PrintObjectList ( scene_t *scene );
obj_model_t *Scene_FindObjectByName( scene_t *scene, char *name );
void         Scene_Destroy         ( scene_t *scene );

#endif // SCENE_H_INCLUDED
