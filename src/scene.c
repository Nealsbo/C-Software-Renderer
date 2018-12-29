#include "srdefs.h"
#include "scene.h"



scene_t *Scene_Init( camera_t *camera ){
    scene_t *scene = (scene_t *)malloc(sizeof(scene_t));
    if(scene == NULL) {
        printf("Error: Cannot create scene!\n");
        return NULL;
    }

    scene->objectList = (List *)malloc(sizeof(List));
    List_Init(scene->objectList, &Model_Test, &Model_Free);

    scene->mainCamera = camera;
    scene->dummLight  = vec4_create(0.0f, 1.0f, 1.0f, 0.0f);
    scene->skybox     = NULL;
    return scene;
}

void Scene_AddObject( scene_t *scene, obj_model_t *model ){
    if( model != NULL ) {
        List_Push ( scene->objectList, model );
    }
}

List *Scene_GetObjectList( scene_t *scene ){
	if(scene->objectList == NULL) {
		return NULL;
	}
    return scene->objectList;
}

void Scene_PrintObjectList( scene_t *scene ){
    List_Print( scene->objectList );
}

obj_model_t *Scene_FindObjectByName( scene_t *scene, char *name ){
	List        *objList = Scene_GetObjectList( scene );
	node_t      *curr    = List_Head( objList );
	obj_model_t *obj     = NULL;
	
	int isFound = FALSE;
	
	while( curr != NULL ){
        if( !strcmp( name, Model_GetName(List_Data(curr)) ) ){
			isFound = TRUE;
			break;
		}
        curr = curr->next;
    }
    if(isFound){
		obj = List_Data(curr);
	} else {
		printf("SceneWarn: Object \"%s\" not found!\n", name);
	}
	return obj;
}

void Scene_Destroy( scene_t *scene ){
    if(scene != NULL) {
        free(scene->mainCamera);
        List_Destroy(scene->objectList);
    }
}
