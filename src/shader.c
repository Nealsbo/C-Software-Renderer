#include "srdefs.h"
#include "shader.h"

vec4 Shader_Vertex( shader_t *shader, vertex_t *vert, int index ) {
	mat4_setColVec2( &shader->var_uv, vert->uv, index );
	shader->vert_nrm = vec4_byMat4( vert->normal, mat4_tsp( mat4_inv( shader->m ) ) );
	shader->light_intens = MAX( 0.0f, vec3_dot( vec4_toVec3( shader->vert_nrm, FALSE ), shader->light_pos ) );
	shader->var_intens.data[index] = MAX( 0.0f, vec4_dot( ( shader->vert_nrm ), vec4_nrm( vec4_sub( vec3_toVec4( shader->light_pos ), vert->position ) ) ) );
	return vec4_byMat4( vert->position, shader->mvp );
}

color_t Shader_Fragment( shader_t *shader, obj_model_t *model, vec3 bary ) {
	float intens = vec3_dot( shader->var_intens, bary );
	vec2  uv     = vec4_toVec2( vec4_byMat4( vec3_toVec4( bary ), shader->var_uv ) );
	return Color_Intensity( Bitmap_GetPixelUV( model->diffmap->bitmap, uv ), intens );
}

void Shader_SetMatrices( shader_t *shader, mat4 model, mat4 mvp ) {
	shader->m = model;
	shader->mvp = mvp;
}

void Shader_SetLight( shader_t *shader, vec3 lpos, vec3 lcolor ) {
	shader->light_pos = lpos;
	shader->light_color = lcolor;
}
