#include "srdefs.h"
#include "render_se.h"

extern renderer_t *renderer_;

static vec3 barycentric( vec2 *a, vec2 p ) {
	int i;
	vec3 s[2];                      // a[0] = a; a[1] = b; a[2] = c;
	s[0].x = a[2].x - a[0].x;       // s[0].x = c.x - a.x;
	s[0].y = a[1].x - a[0].x;       // s[0].y = b.x - a.x;
	s[0].z = a[0].x - p.x;          // s[0].z = a.x - p.x;
	s[1].x = a[2].y - a[0].y;       // s[1].x = c.y - a.y;
	s[1].y = a[1].y - a[0].y;       // s[1].y = b.y - a.y;
	s[1].z = a[0].y - p.y;          // s[1].z = a.y - p.y;
	vec3 u = vec3_crs(s[0], s[1]);
	if( fabs(u.y) > 0.01f ) {
		vec3_create(1.0f - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z);
	}
	return vec3_create(-1.0f, 1.0f, 1.0f);
}

float TriangleAreaTimesTwo( vertex_t *a, vertex_t *b, vertex_t *c ) {
    float x1 = b->position.x - a->position.x;
    float y1 = b->position.y - a->position.y;
    float x2 = c->position.x - a->position.x;
    float y2 = c->position.y - a->position.y;
    return ( x1 * y2 - x2 * y1 );
}

static void Render_DrawTriangle( vertex_t *v, SDL_Surface *Surface, obj_model_t *model ) {
	int i;
	vec2 p;
	unsigned int srcX, srcY = 0;
	
	vec2 pts[3] = { vec4_toVec2(v[0].position),
		             vec4_toVec2(v[1].position),
		             vec4_toVec2(v[2].position) };
	
	vec2 bboxmin = vec2_create(  FLT_MAX,  FLT_MAX );
	vec2 bboxmax = vec2_create( -FLT_MAX, -FLT_MAX );
	vec2 clamp   = vec2_create( WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 );

	for(i = 0; i < 3; i++) {
		bboxmin.x = MAX(       0, MIN( bboxmin.x, v[i].position.x ) );
		bboxmax.x = MIN( clamp.x, MAX( bboxmax.x, v[i].position.x ) );
		bboxmin.y = MAX(       0, MIN( bboxmin.y, v[i].position.y ) );
		bboxmax.y = MIN( clamp.y, MAX( bboxmax.y, v[i].position.y ) );
	}
	
	for( p.x = bboxmin.x; p.x <= bboxmax.x; p.x++ ) {
		for( p.y = bboxmin.y; p.y <= bboxmax.y; p.y++ ) {
			vec3 c = barycentric(pts, p);
			
			float z = v[0].position.z * c.x + 
			          v[1].position.z * c.y +
			          v[2].position.z * c.z;
			float w = v[0].position.w * c.x +
			          v[1].position.w * c.y +
			          v[2].position.w * c.z;
			
			if( c.x < 0 || c.y < 0 || c.z < 0 ) continue;
			renderer_->PutPixel( Surface, p.x, p.y, Color_ToUInt32( Bitmap_GetPixel(model->diffmap->bitmap, srcX, srcY) ) );
		}
	}
}

static void Render_DrawLine(vertex_t v0, vertex_t v1, SDL_Surface *image, color_t color) {
    mat4 sst = mat4_screen ( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 );
    vec2i p0 = vec4_toVec2i(vec4_pdiv(vec4_byMat4(v0.position, sst)));
    vec2i p1 = vec4_toVec2i(vec4_pdiv(vec4_byMat4(v1.position, sst)));

    int x0 = p0.x, y0 = p0.y, x1 = p1.x, y1 = p1.y;

    int deltax = abs( x1 - x0 ), signx = x0 < x1 ? 1 : -1;
    int deltay = abs( y1 - y0 ), signy = y0 < y1 ? 1 : -1;
    int error2, error = ( deltax > deltay ? deltax : -deltay) / 2;

    while(1) {
        if( x0 >= 0 && y0 >= 0 && x0 < image->w && y0 < image->h )
            renderer_->PutPixel( image, x0, y0, Color_ToUInt32(color) );
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

void Render_SB_DrawObject( scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface ) {
    int i, j;
    obj_face_t face;
    vertex_t vertex[3];
    
    renderer_ = renderer;
    camera_t *cam = scene->mainCamera;

    mat4 worldToCamera = mat4_lookAt( cam->position, cam->front, vec3_create(0.0, 1.0, 0.0) );

    mat4 translation   = mat4_translate( model->position.x, model->position.y, model->position.z );
    mat4 rotation      = mat4_rotation( model->rotation.x, model->rotation.y, model->rotation.z );
    mat4 scale         = mat4_scale( model->scale.x, model->scale.y, model->scale.z );
    mat4 modelMat      = mat4_mlt( mat4_mlt(translation, rotation), scale );

    mat4 cameraMat     = mat4_mlt( worldToCamera, modelMat );
    mat4 viewMat       = mat4_mlt( scene->mainCamera->projection, cameraMat );

    for(i = 0; i < model->trisCount; i++) {
        face = model->faces[i];
        for(j = 0; j < face.vcount; j++) {
            vertex[j] = Vertex_Init( vec4_byMat4( vec3_toVec4(model->vertices[face.indexes[j].x - 1]), viewMat ),
                                     vec4_byMat4( vec3_toVec4(model->normals[face.indexes[j].z - 1]), rotation ),
                                     model->textcoords[face.indexes[j].y - 1] );
        }

        switch(renderer->flagState) {
            case RENDER_STATE_WIREFRAME:
                Render_DrawLine( vertex[0], vertex[1], Surface, model->baseColor );
                Render_DrawLine( vertex[1], vertex[2], Surface, model->baseColor );
                Render_DrawLine( vertex[0], vertex[2], Surface, model->baseColor );
                break;

            case RENDER_STATE_LIT:
                Render_DrawTriangle( vertex, Surface, model );
                break;

            default:
                break;
        }
    }
}
