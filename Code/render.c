#include "srdefs.h"
#include "render.h"

//############################
//###   Raster functions   ###
//############################

void Edge_Init(edge_t edge, vec3 miny, vec3 maxy){
    edge.yStart = (int)ceilf(miny.y);
    edge.yEnd   = (int)ceilf(maxy.y);

    float yDist = maxy.y - miny.y;
    float xDist = maxy.x - miny.x;

    float yPrestep = edge.yStart - miny.y;
    edge.xStep = (float)xDist/(float)yDist;
    edge.x = miny.x + yPrestep * edge.xStep;
}

void Edge_Step(edge_t *edge){
    edge->x += edge->xStep;
}

void Edge_Print(edge_t edge){
    printf("%f, %f\n%d, %d\n", edge.x, edge.xStep, edge.yStart, edge.yEnd);
}

void Gradient_Init(gradient_t grad, vec3 miny, vec3 midy, vec3 maxy){
}

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
    List_Push ( scene->objectList, model );
}

List *Scene_GetObjectList( scene_t *scene ){
    return scene->objectList;
}

void Scene_PrintObjectList( scene_t *scene ){
    List_Print( scene->objectList );
}

void Scene_Destroy(scene_t *scene){
    Model_Free(scene->skybox);
    free(scene->mainCamera);
    List_Destroy(scene->objectList);
}

//###############################
//###   Rendering functions   ###
//###############################

renderContext_t *Render_InitContext(uint32_t flag){
    renderContext_t *rc = malloc(sizeof(renderContext_t));
    rc->flagState  = flag;
    rc->fog        = 0;
    rc->fogIntence = 0.0;
    return rc;
}

renderInfo_t *Render_Init(renderContext_t *context, uint32_t flag){
    renderInfo_t *render = malloc(sizeof(renderInfo_t));
    context = Render_InitContext(flag);
    render->context = context;
    render->z_Buffer = Bitmap_CreateMap(WINDOW_WIDTH, WINDOW_HEIGHT);
    render->frameCount = 0;
    render->frameTime = 0.0;
    return render;
}

void Render_SwitchRenderState(renderInfo_t *render){
    if(render->context->flagState == RENDER_STATE_WIREFRAME)
        render->context->flagState = RENDER_STATE_LIT;
    else if(render->context->flagState == RENDER_STATE_LIT)
        render->context->flagState = RENDER_STATE_WIREFRAME;
}

void Render_UpdateRenderInfo(renderInfo_t *render){} //TODO

void putpixel(SDL_Surface *surface, int x, int y, uint32_t pixel){
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

void Render_DrawLine(vec2i p0, vec2i p1, SDL_Surface *image, color_t color) {
    int x0 = p0.x, y0 = p0.y, x1 = p1.x, y1 = p1.y;

    int deltax = abs( x1 - x0 ), signx = x0 < x1 ? 1 : -1;
    int deltay = abs( y1 - y0 ), signy = y0 < y1 ? 1 : -1;
    int error2, error = ( deltax > deltay ? deltax : -deltay) / 2;

    while(1) {
        if( x0 >= 0 && y0 >= 0 && x0 < image->w && y0 < image->h )
            putpixel( image, x0, y0, Color_ToUInt32(color) );
        if( x0 == x1 && y0 == y1 )
            break;

        error2 = error;
        if ( error2 >- deltax ){
            error -= deltay;
            x0    += signx;
        }
        if ( error2 <  deltay ){
            error += deltax;
            y0    += signy;
        }
    }
}

void Render_DrawTriangle(vec2i t0, vec2i t1, vec2i t2, SDL_Surface *image, color_t color) {
    if (t0.y == t1.y && t0.y == t2.y)
        return;
    if (t0.y > t1.y)
        SWAP_STRUCT(&t0, &t1, vec2i);
    if (t0.y > t2.y)
        SWAP_STRUCT(&t0, &t2, vec2i);
    if (t1.y > t2.y)
        SWAP_STRUCT(&t1, &t2, vec2i);
    int i, j, height = t2.y - t0.y;
    for (i = 0; i < height; i++) {
        int half     = i > t1.y - t0.y || t1.y == t0.y;
        int s_height = half ? t2.y - t1.y : t1.y - t0.y;
        float alpha  = (float)i/height;
        float beta   = (float)(i-(half ? t1.y - t0.y : 0))/s_height;
        vec2i a =        vec2i_add(t0, vec2i_mlt(vec2i_sub(t2, t0), alpha));
        vec2i b = half ? vec2i_add(t1, vec2i_mlt(vec2i_sub(t2, t1),  beta)) : vec2i_add(t0, vec2i_mlt(vec2i_sub(t1, t0), beta));
        if (a.x > b.x)
            SWAP_STRUCT(&a, &b, vec2i);
        for (j = a.x; j <= b.x; j++) {
            putpixel(image, j, t0.y + i, Color_ToUInt32(color));
        }
    }
}

void Render_DrawObject(scene_t *scene, renderInfo_t *render, obj_model_t *model, SDL_Surface *Surface, float delta){
    int i, j;
    vec2i texi[3];
    vec3 verts[3];
    obj_face_t face;

    Model_SetPosition(model, (vec3){0.0, 0.0, -4.0});
    Model_SetRotation(model, (vec3){0.0, delta, 0.0});

    mat4 translation           = mat4_translate( model->position.x, model->position.y, model->position.z );
    mat4 rotation              = mat4_rotation( model->rotation.x, model->rotation.y, model->rotation.z );
    mat4 transformation        = mat4_mlt( scene->mainCamera->projection, mat4_mlt( translation, rotation ) );
    mat4 screenSpaceTransform  = mat4_screen(Surface->w/2, Surface->h/2);

    for(i = 0; i < model->trisCount; i++){
        face = model->faces[i];
        for(j = 0; j < face.vcount; j++){
            verts[j] = vec3_byMat4(model->vertices[face.indexes[j].x - 1], transformation);
            texi[j]  = vec3_toVec2i(vec3_byMat4(verts[j], screenSpaceTransform));
        }

        switch(render->context->flagState){
            case RENDER_STATE_WIREFRAME:
                Render_DrawLine(texi[0], texi[1], Surface, model->baseColor);
                Render_DrawLine(texi[1], texi[2], Surface, model->baseColor);
                Render_DrawLine(texi[0], texi[2], Surface, model->baseColor);
                break;

            case RENDER_STATE_LIT:

                Render_DrawTriangle( texi[0], texi[1], texi[2], Surface, model->baseColor );
                break;

            default:
                break;
        }
    }
}

void Render_DrawWorld(scene_t *scene, renderInfo_t *render, SDL_Surface *Surface, float delta){
    node_t *objList_node = List_Head( Scene_GetObjectList(scene) );

    while(objList_node != NULL){
        Render_DrawObject(scene, render, List_Data(objList_node), Surface, delta);
        objList_node = objList_node->next;
    }
    render->frameCount++;
}

void Render_Destroy(renderInfo_t *render, renderContext_t *context){
    free(context);
    render->context = NULL;
    Bitmap_Free(render->z_Buffer);
    render->z_Buffer = NULL;
    printf("\nRendered frames: %d\n", render->frameCount);
    free(render);
}
