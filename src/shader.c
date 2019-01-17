#include "srdefs.h"
#include "shader.h"

vec4 Shader_Vertex( shader_t *shader, vertex_t *vert, int index ) {
	//mat4_setColVec2( &shader->var_uv, vert->uv, index );
	//mat4_setColVec4( &shader->var_nrm, vec4_byMat4( vert->normal, shader->m_it ), index );
	//mat4_setColVec4( &shader->var_pos, vec4_byMat4( vert->position, shader->m ), index );
	shader->pos[index]     = vec4_byMat4( vert->position, shader->m );
	shader->normals[index] = vec4_byMat4( vert->normal, shader->m_it );
	shader->uvs[index]     = vert->uv;
	return vec4_byMat4( vert->position, shader->mvp );
}

color_t Shader_Fragment( shader_t *s, obj_model_t *model, vec3 bary ) {
	//vec2 uv      = vec4_toVec2( vec4_byMat4( vec3_toVec4( bary ), s->var_uv ) );
	vec2 uv      = vec2_create( bary.x*s->uvs[0].x + bary.y*s->uvs[1].x + bary.z*s->uvs[2].x,
                                bary.x*s->uvs[0].y + bary.y*s->uvs[1].y + bary.z*s->uvs[2].y );
                                
	if( !s->isLit ) {
		return Bitmap_GetPixelUV( model->diffmap->bitmap, uv );
	}
	float ambientStr = 0.1f, specularStr = 1.0f;
	//vec3 fragPos = vec3_nrm( vec3_byMat4( bary, s->var_pos ) );
	//vec3 normal  = vec3_nrm( vec3_byMat4( bary , s->var_nrm ) );
	vec3 fragPos = vec3_nrm( vec3_create( bary.x*s->pos[0].x + bary.y*s->pos[1].x + bary.z*s->pos[2].x,
										  bary.x*s->pos[0].y + bary.y*s->pos[1].y + bary.z*s->pos[2].y,
										  bary.x*s->pos[0].z + bary.y*s->pos[1].z + bary.z*s->pos[2].z ) );
                                          
	vec3 normal  = vec3_nrm( vec3_create( bary.x*s->normals[0].x + bary.y*s->normals[1].x + bary.z*s->normals[2].x + s->normals[3].x,
										  bary.x*s->normals[0].y + bary.y*s->normals[1].y + bary.z*s->normals[2].y + s->normals[3].y,
										  bary.x*s->normals[0].z + bary.y*s->normals[1].z + bary.z*s->normals[2].z + s->normals[3].z) );
	
	// ambient light
	vec3 ambient    = vec3_mlt( s->ambient_color, ambientStr );
	
	// diffuse light
	vec3  light_dir = vec3_nrm( vec3_sub( s->light_pos, fragPos ) );
	float diff      = MAX( vec3_dot( normal, light_dir ), 0.0f );
	vec3  diffuse   = vec3_mlt( s->light_color, diff );
	
	// specular light
	vec3  view_dir  = vec3_nrm( vec3_sub( s->cam_pos, fragPos ) );
	vec3  refl_dir  = vec3_nrm( vec3_refl( vec3_neg( light_dir ), normal ) );
	float spec      = powf( MAX( vec3_dot( view_dir, refl_dir ), 0.0f ), 8.0f );
	vec3  specular  = vec3_mlt( s->light_color, specularStr * spec );
	
	vec3  res       = vec3_add( vec3_add( vec3_mlt( diffuse, 0.5f ), ambient ), specular );
	
	return Color_IntensityVec3( Bitmap_GetPixelUV( model->diffmap->bitmap, uv ), res );
}

void Shader_UseLight( shader_t *shader, int to_use ) {
	shader->isLit = to_use;
}

void Shader_SetMatrices( shader_t *shader, mat4 model, mat4 view, mat4 mvp ) {
	shader->m    = model;
	shader->v    = view;
	shader->m_it = mat4_tsp( mat4_inv( shader->m ) );
	shader->mvp  = mvp;
}

void Shader_SetCamera( shader_t *shader, vec3 cam_pos ) {
	shader->cam_pos = cam_pos;
}

void Shader_SetLight( shader_t *shader, vec3 dlpos, vec3 dlcolor, vec3 alcolor ) {
	shader->light_pos = dlpos;
	shader->light_color = dlcolor;
	shader->ambient_color = alcolor;
}
