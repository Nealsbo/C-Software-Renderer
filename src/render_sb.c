#include "srdefs.h"
#include "render_se.h"

extern renderer_t *renderer_;

static vec3f barycentric(vec2f a, vec2f b, vec2f c, vec2f p){
	int i;
	vec3f s[];
	s[0].x = c.x - a.x;
	s[0].y = b.x - a.x;
	s[0].z = a.x - p.x;
	s[1].x = c.y - a.y;
	s[1].y = b.y - a.y;
	s[1].z = a.y - p.y;
	vec3f u = vec3f_crs(s[0], s[1]);
	if(fabs(u.y) > .01f){
		vec3_create(1.f - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z);
	}
	return vec3_create(-1.f, 1.f, 1.f);
}

float TriangleAreaTimesTwo( vertex_t *a, vertex_t *b, vertex_t *c ){
    float x1 = b->position.x - a->position.x;
    float y1 = b->position.y - a->position.y;
    float x2 = c->position.x - a->position.x;
    float y2 = c->position.y - a->position.y;
    return ( x1 * y2 - x2 * y1 );
}

static void Render_DrawTriangle( vertex_t *miny, vertex_t *midy, vertex_t *maxy, SDL_Surface *Surface, obj_model_t *model ){
	int i, j;

	for (int i=0; i<3; i++){
		pts2[i] = proj<2>(pts[i]/pts[i][3]);
	}

	vec2i p;
	vec2f bboxmin = vec2i_create(FLT_MAX, -FLT_MAX);
	vec2f bboxmax = vec2i_create(-FLT_MAX, FLT_MAX);
	vec2f clamp = vec2i_create(WINDOW_WIDTH-1, WINDOW_HEIGHT-1);

	for(i = 0; i < 3; i++){
		bboxmin.x = MAX(0, MIN(bboxmin.x, pts[i].x));
		bboxmax.x = MIN(clamp.x, MAX(bboxmax.x, pts[i].x));
		bboxmin.y = MAX(0, MIN(bboxmin.y, pts[i].x));
		bboxmax.y = MIN(clamp.y, MAX(bboxmax.y, pts[i].y));
	}
	for(p.x = bboxmin.x; p.x <= bboxmax.x; p.x++){
		for(p.y = bboxmin.y; p.y <= bboxmax.y; p.y++){
			vec3f bc_scrn = barycentric(pts, p);
			if(bc_scrn.x < 0 || bc_scrn.y < 0 || bc_scrn.z < 0) continue;
			renderer_->PutPixel(Surface, p.x, p.y, Color_ToUInt32(Bitmap_GetPixel(texture, srcX, srcY)));
		}
	}
}

void Render_SE_DrawObject(scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface, float delta){
    int i, j;
    obj_face_t face;
    vertex_t vertex[3];
    
    renderer_ = renderer;
    Model_SetRotation(model, vec3_create(0.0, delta, 0.0));
    camera_t *cam = scene->mainCamera;

    mat4 worldToCamera = mat4_lookAt(cam->position, cam->front, vec3_create(0.0, 1.0, 0.0));

    mat4 translation   = mat4_translate( model->position.x, model->position.y, model->position.z );
    mat4 rotation      = mat4_rotation( model->rotation.x, model->rotation.y, model->rotation.z );
    mat4 scale         = mat4_scale( model->scale.x, model->scale.y, model->scale.z );
    mat4 modelMat      = mat4_mlt( mat4_mlt(translation, rotation), scale );

    mat4 cameraMat     = mat4_mlt( worldToCamera, modelMat );
    mat4 viewMat       = mat4_mlt( scene->mainCamera->projection, cameraMat );

    for(i = 0; i < model->trisCount; i++){
        face = model->faces[i];
        for(j = 0; j < face.vcount; j++){
            vertex[j] = Vertex_Init( vec4_byMat4(vec3_toVec4(model->vertices[face.indexes[j].x - 1]), viewMat),
                                     model->normals[face.indexes[j].z - 1],
                                     model->textcoords[face.indexes[j].y - 1] );
        }

        switch(renderer->flagState){
            case RENDER_STATE_WIREFRAME:
                Render_DrawLine( vertex[0], vertex[1], Surface, model->baseColor );
                Render_DrawLine( vertex[1], vertex[2], Surface, model->baseColor );
                Render_DrawLine( vertex[0], vertex[2], Surface, model->baseColor );
                break;

            case RENDER_STATE_LIT:
                Render_DrawTriangle( &vertex[0], &vertex[1], &vertex[2], Surface, model );
                break;

            default:
                break;
        }
    }
}
