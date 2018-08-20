#include "srdefs.h"
#include "render_rt.h"

extern renderer_t *renderer_;

static bool Render_RayTriangleInters( ray *r, vertex_t *v0, vertex_t *v1, vertex_t *v2, ray_hit_t *hit, SDL_Surface *image, obj_model_t *model ){
	vec3 v0v1 = vec3_sub(vec4_toVec3(v1->position, 1), vec4_toVec3(v0->position, 1));
	vec3 v0v2 = vec3_sub(vec4_toVec3(v2->position, 1), vec4_toVec3(v0->position, 1));
	vec3 pvec = vec3_crs(r->direction, v0v2);
	float det = vec3_dot(v0v1, pvec);
	
	if(det < EPS)
		return FALSE;
	
	float invDet = 1.0f / det;
	
	vec3 tvec = vec3_sub(r->origin, vec4_toVec3(v0->position, 1));
	hit->u = vec3_dot(tvec, pvec) * invDet;
	if(hit->u < 0.0f || hit->u > 1.0f)
		return TRUE;
	
	vec3 qvec = vec3_crs(tvec, v0v1);
	hit->v = vec3_dot(r->direction, qvec) * invDet;
	if(hit->v < 0.0f || hit->u + hit->v > 1.0f)
		return FALSE;
	
	hit->t = vec3_dot(v0v2, qvec) * invDet;
	
    return TRUE;
}

static void Render_DrawTriangle( vertex_t *miny, vertex_t *midy, vertex_t *maxy, SDL_Surface *Surface, obj_model_t *model ){

}

void Render_RT_DrawObject(scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface, float delta){
	
} 
