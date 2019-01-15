#include "srdefs.h"
#include "shader.h"

vec4 Shader_Vertex( shader_t *shader, vertex_t *vert, int index ) {
	mat4_setColVec2( &shader->var_uv, vert->uv, index );
	mat4_setColVec4( &shader->var_nrm, vec4_byMat4( vert->normal, shader->m_it ), index );
	mat4_setColVec4( &shader->var_pos, vec4_byMat4( vert->position, shader->m ), index );
	return vec4_byMat4( vert->position, shader->mvp );
}

color_t Shader_Fragment( shader_t *shader, obj_model_t *model, vec3 bary ) {
	float ambientStr = 0.1f, specularStr = 1.0f;
	vec3 fragPos = vec3_nrm( vec3_byMat4( bary, shader->var_pos ) );
	
	// ambient light
	vec3 ambient = vec3_mlt( vec3_create( 0.0f, 0.5f, 1.0f ), ambientStr );
	
	// diffuse light
	vec3  normal    = vec3_nrm( vec3_byMat4( bary , shader->var_nrm ) );
	vec3  light_dir = vec3_nrm( vec3_sub( shader->light_pos, fragPos ) );
	float diff      = MAX( vec3_dot( normal, light_dir ), 0.0f );
	vec3  diffuse   = vec3_mlt( shader->light_color, diff );
	
	// specular light
	vec3  view_dir = vec3_nrm( vec3_sub( shader->cam_pos, fragPos ) );
	vec3  refl_dir = vec3_nrm( vec3_refl( vec3_neg( light_dir ), normal ) );
	float spec     = powf( MAX( vec3_dot( view_dir, refl_dir ), 0.0f ), 16.0f );
	vec3  specular = vec3_mlt( shader->light_color, specularStr * spec );
	
	vec3  res    = vec3_add( vec3_add( vec3_mlt( diffuse, 0.5f ), ambient ), specular );
	vec2  uv     = vec4_toVec2( vec4_byMat4( vec3_toVec4( bary ), shader->var_uv ) );
	
	return Color_IntensityVec3( Bitmap_GetPixelUV( model->diffmap->bitmap, uv ), res );
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

void Shader_SetLight( shader_t *shader, vec3 lpos, vec3 lcolor ) {
	shader->light_pos = lpos;
	shader->light_color = lcolor;
}
