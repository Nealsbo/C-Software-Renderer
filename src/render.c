#include "srdefs.h"
#include "render.h"
#include "raster.h"

renderer_t *renderer_ = NULL;

//###############################
//###   Rendering functions   ###
//###############################

renderer_t *Renderer_Init( scene_t *scene, uint32_t rs_flag, uint32_t rt_flag ) {
    renderer_t *renderer = malloc( sizeof(renderer_t) );
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

void Renderer_SwitchRendState( renderer_t *renderer ) {
    if( renderer->flagState == RENDER_STATE_WIREFRAME )
        renderer->flagState = RENDER_STATE_LIT;
    else if( renderer->flagState == RENDER_STATE_LIT )
        renderer->flagState = RENDER_STATE_WIREFRAME;
}

void Renderer_Update( renderer_t *renderer ){} //TODO

void Renderer_ClearZBuffer( renderer_t *renderer ) {
    int i;
    for( i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++ )
		renderer->z_Buffer[i] = FLT_MAX;
}

void Renderer_Putpixel( SDL_Surface *surface, int x, int y, uint32_t pixel ) {
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

void Renderer_DrawObject( scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface ) {
    int i, j;
    obj_face_t face;
    vec4 rast_verts[3];
    vertex_t vertex[3], vertexx[3];
    
    renderer_       = renderer;
    camera_t *cam   = scene->mainCamera;
    shader_t shader;
    
    Shader_SetLight( &shader, scene->dummLight, vec3_create( 1.0f, 0.0f, 0.0f ) );

    mat4 worldToCamera = mat4_lookAt( cam->position, cam->front, vec3_create( 0.0, 1.0, 0.0 ) );

    mat4 translation   = mat4_translate( model->position.x, model->position.y, model->position.z );
    mat4 rotation      = mat4_rotation( model->rotation.x, model->rotation.y, model->rotation.z );
    mat4 scale         = mat4_scale( model->scale.x, model->scale.y, model->scale.z );
    mat4 modelMat      = mat4_mlt( mat4_mlt(translation, rotation), scale );

    mat4 cameraMat     = mat4_mlt( worldToCamera, modelMat );
    mat4 viewMat       = mat4_mlt( scene->mainCamera->projection, cameraMat );

    for( i = 0; i < model->trisCount; i++ ) {
        face = model->faces[i];
        for( j = 0; j < face.vcount; j++ ) {
            vertex[j]     = Vertex_Init( vec3_toVec4( model->vertices  [face.indexes[j].x - 1] ),
                                         vec3_toVec4( model->normals   [face.indexes[j].z - 1] ),
                                         model->textcoords[face.indexes[j].y - 1] );

            rast_verts[j] = Shader_Vertex( &shader, &vertex[j], viewMat, j );
        }

        switch( renderer->flagState ) {
            case RENDER_STATE_WIREFRAME:
                Raster_DrawLine( rast_verts[0], rast_verts[1], Surface, model->baseColor );
                Raster_DrawLine( rast_verts[1], rast_verts[2], Surface, model->baseColor );
                Raster_DrawLine( rast_verts[0], rast_verts[2], Surface, model->baseColor );
                break;

            case RENDER_STATE_LIT:
                Raster_DrawTriangle( rast_verts, Surface, model, &shader );
                break;

            default:
                break;
        }
    }
}

void Renderer_DrawWorld( renderer_t *renderer, SDL_Surface *Surface ) {
	scene_t *scene = renderer->scene;
    SDL_FillRect( Surface, NULL, SDL_MapRGB( Surface->format, 0x00, 0x00, 0x00 ) );
    Renderer_ClearZBuffer( renderer );
    node_t *objList_node = List_Head( Scene_GetObjectList( scene ) );

    while( objList_node != NULL ) {
        Renderer_DrawObject( scene, renderer, List_Data(objList_node), Surface );
        objList_node = objList_node->next;
    }
    renderer->frameCount++;
}

void Renderer_Destroy( renderer_t *renderer ) {
    if( renderer != NULL ) {
        free( renderer->z_Buffer );
        renderer->z_Buffer = NULL;
        printf( "\nRendered frames: %d\n", renderer->frameCount );
        free( renderer );
    }
}
