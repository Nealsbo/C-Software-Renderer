#include "srdefs.h"
#include "render.h"

renderer_t *renderer_ = NULL;

//###########################
//###   Scene functions   ###
//###########################

scene_t *Scene_Init( camera_t *camera ){
    scene_t *scene = (scene_t *)malloc(sizeof(scene_t));
    if(scene == NULL){
        printf("Error: Cannot create scene!\n");
        return NULL;
    }

    scene->objectList = (List *)malloc(sizeof(List));
    List_Init(scene->objectList, &Model_Test, &Model_Free);

    scene->mainCamera = camera;
    scene->skybox     = NULL;
    return scene;
}

void Scene_AddObject( scene_t *scene, obj_model_t *model ){
    if( model != NULL ){
        List_Push ( scene->objectList, model );
    }
}

List *Scene_GetObjectList( scene_t *scene ){
    return scene->objectList;
}

void Scene_PrintObjectList( scene_t *scene ){
    List_Print( scene->objectList );
}

void Scene_Destroy( scene_t *scene ){
    if(scene != NULL) {
        free(scene->mainCamera);
        List_Destroy(scene->objectList);
    }
}

//###############################
//###   Rendering functions   ###
//###############################

renderer_t *Renderer_Init(scene_t *scene, uint32_t rs_flag, uint32_t rt_flag){
    renderer_t *renderer = malloc(sizeof(renderer_t));
    renderer->scene      = scene;
    renderer->flagState  = rs_flag;
    renderer->renderType = rt_flag;
    renderer->fog        = 0;
    renderer->fogIntence = 0.0;
    renderer->z_Buffer   = (float *)malloc(sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT);
    renderer->frameCount = 0;
    renderer->frameTime  = 0.0;
    renderer->PutPixel   = Renderer_Putpixel;
    return renderer;
}

void Renderer_SwitchRendState( renderer_t *renderer ){
    if( renderer->flagState == RENDER_STATE_WIREFRAME )
        renderer->flagState = RENDER_STATE_LIT;
    else if( renderer->flagState == RENDER_STATE_LIT )
        renderer->flagState = RENDER_STATE_WIREFRAME;
}

void Renderer_Update(renderer_t *renderer){} //TODO

void Renderer_ClearZBuffer(renderer_t *renderer){
    int i;
    for( i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++ ) renderer->z_Buffer[i] = FLT_MAX;
}

void Renderer_Putpixel(SDL_Surface *surface, int x, int y, uint32_t pixel){
    if ( x > surface->w || x < 0 || y > surface->h || y < 0 )
        return;
    int bpp = surface->format->BytesPerPixel;
    uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp) {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(uint16_t *)p = pixel;
        break;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(uint32_t *)p = pixel;
        break;
    }
}

void Renderer_DrawWorld(scene_t *scene, renderer_t *renderer, SDL_Surface *Surface, float delta){
    SDL_FillRect( Surface, NULL, SDL_MapRGB( Surface->format, 0x00, 0x00, 0x00 ) );
    Renderer_ClearZBuffer(renderer);
    node_t *objList_node = List_Head( Scene_GetObjectList(scene) );

    while(objList_node != NULL){
        Render_SE_DrawObject(scene, renderer, List_Data(objList_node), Surface, delta);
        objList_node = objList_node->next;
    }
    renderer->frameCount++;
}

void Renderer_Destroy(renderer_t *renderer){
    if(renderer != NULL) {
        free(renderer->z_Buffer);
        renderer->z_Buffer = NULL;
        printf("\nRendered frames: %d\n", renderer->frameCount);
        free(renderer);
    }
}
