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
    if( renderer->flagState == RENDER_STATE_WIREFRAME ) {
        renderer->flagState = RENDER_STATE_LIT;
    } else if( renderer->flagState == RENDER_STATE_LIT ) {
        renderer->flagState = RENDER_STATE_UNLIT;
    } else if( renderer->flagState == RENDER_STATE_UNLIT ) {
        renderer->flagState = RENDER_STATE_Z_BUFFER;
    } else if( renderer->flagState == RENDER_STATE_Z_BUFFER ) {
        renderer->flagState = RENDER_STATE_WIREFRAME;
    }
}

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

int Renderer_ClipTriangle( vec4 *v ) {
	int i, res = 0;
	for( i = 0; i < 3; i++ ) {
		if( ( v[i].x < -1.0f && v[i].x > 1.0 ) &&
		    ( v[i].y < -1.0f && v[i].y > 1.0 ) &&
		    ( v[i].z <  0.0f && v[i].z > 1.0 ) )
		    res++;
	}
	return res == 3 ? 1 : 0;
}

void Renderer_DrawObject( scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface ) {
    int i, j;
    obj_face_t face;
    vec4 rast_verts[3];
    vertex_t vertex[3], vertexx[3];
    mat4 camMat = {0.0f}, lookAtMat = {0.0f}, tmp = {0.0f};
    mat4 translation = {0.0f}, rotation = {0.0f}, scale = {0.0f};
    mat4 modelMat = {0.0f}, viewMat = {0.0f}, projMat = {0.0f};
    
    renderer_       = renderer;
    camera_t *cam   = scene->mainCamera;
    shader_t shader = { 0 };
    
    Shader_SetLight( &shader, scene->directLight, scene->directLightColor, scene->ambientLightColor );

    mat4_lookAt( lookAtMat,
                 Camera_GetPosition( cam ),
                 vec3_add( Camera_GetPosition( cam ), Camera_GetFront( cam ) ),
                 vec3_create( 0.0, 1.0, 0.0 ) );

    mat4_translate( translation, model->position.x, model->position.y, model->position.z );
    mat4_rotation(  rotation,    model->rotation.x, model->rotation.y, model->rotation.z );
    mat4_scale(     scale,       model->scale.x,    model->scale.y,    model->scale.z );
    
    mat4_mlt( translation, rotation, tmp );
    mat4_mlt( tmp, scale, modelMat );
    mat4_mlt( lookAtMat, modelMat, viewMat );
    mat4_mlt( scene->mainCamera->projection, viewMat, projMat );

    Shader_SetMatrices( &shader, modelMat, lookAtMat, projMat );
    Shader_SetCamera( &shader, Camera_GetPosition( cam ) );
    Shader_InvertNormal( &shader, TRUE );

    for( i = 0; i < model->trisCount; i++ ) {
        face = model->faces[i];
        for( j = 0; j < face.vcount; j++ ) {
            vertex[j]     = Vertex_Init( vec3_toVec4( model->vertices  [face.indexes[j].x - 1] ),
                                         vec3_toVec4( model->normals   [face.indexes[j].z - 1] ),
                                         model->textcoords[face.indexes[j].y - 1] );

            rast_verts[j] = Shader_Vertex( &shader, &vertex[j], j );
        }
        
        if( Renderer_ClipTriangle( rast_verts ) )
			continue;

        switch( renderer->flagState ) {
            case RENDER_STATE_WIREFRAME:
                Raster_DrawLine( rast_verts[0], rast_verts[1], Surface, model->baseColor );
                Raster_DrawLine( rast_verts[1], rast_verts[2], Surface, model->baseColor );
                Raster_DrawLine( rast_verts[0], rast_verts[2], Surface, model->baseColor );
                break;
                
            case RENDER_STATE_UNLIT:
				Shader_UseLight( &shader, FALSE );
				Raster_DrawTriangle( rast_verts, Surface, model, &shader );
				break;

            case RENDER_STATE_LIT:
				Shader_UseLight( &shader, TRUE );
                Raster_DrawTriangle( rast_verts, Surface, model, &shader );
                break;
                
            case RENDER_STATE_Z_BUFFER:
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
