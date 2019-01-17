#include "srdefs.h"
#include "scene.h"



scene_t *Scene_Init( camera_t *camera ) {
    scene_t *scene = (scene_t *)malloc(sizeof(scene_t));
    if(scene == NULL) {
        printf( "Error: Cannot create scene!\n" );
        return NULL;
    }

    scene->objectList = (List *)malloc(sizeof(List));
    List_Init( scene->objectList, &Model_Test, &Model_Free );

    scene->mainCamera   = camera;
    scene->directLight = vec3_create( 0.0f, 5.0f, 5.0f );
	scene->directLightColor = vec3_create( 1.0f, 1.0f, 1.0f );
	scene->ambientLightColor = vec3_create( 1.0f, 1.0f, 1.0f );
    scene->currentState = SCENE_STATE_UPDATABLE;
    return scene;
}

void Scene_AddObject( scene_t *scene, obj_model_t *model ) {
    if( model != NULL ) {
        List_Push ( scene->objectList, model );
    }
}

List *Scene_GetObjectList( scene_t *scene ) {
	if( scene->objectList == NULL ) {
		return NULL;
	}
    return scene->objectList;
}

void Scene_SetDirectLight( scene_t *scene, vec3 dir, vec3 color ) {
	scene->directLight = dir;
	scene->directLightColor = color;
}
void Scene_SetAmbientLight( scene_t *scene, vec3 color ) {
	scene->ambientLightColor = color;
}

void Scene_PrintObjectList( scene_t *scene ) {
    List_Print( scene->objectList );
}

obj_model_t *Scene_FindObjectByName( scene_t *scene, char *name ) {
	List        *objList = Scene_GetObjectList( scene );
	node_t      *curr    = List_Head( objList );
	obj_model_t *obj     = NULL;
	
	int isFound = FALSE;
	
	while( curr != NULL ) {
		if( strstr( Model_GetName(List_Data(curr)), name ) ) {
			isFound = TRUE;
			break;
		}
        curr = curr->next;
    }
    if(isFound){
		obj = List_Data(curr);
	} else {
		printf( "SceneWarn: Object \"%s\" not found!\n", name );
	}
	return obj;
}

void Scene_Destroy( scene_t *scene ) {
    if( scene != NULL ) {
        free(scene->mainCamera);
        List_Destroy(scene->objectList);
    }
}
