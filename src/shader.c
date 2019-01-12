#include "srdefs.h"
#include "shader.h"

vec4 Shader_Vertex( shader_t *shader, vertex_t *vert, mat4 _mvp, int index ) {
	shader->mvp = _mvp;
	mat4_setColVec2( &shader->var_uv, vert->uv, index );
	shader->vert_nrm = vec4_toVec3( vert->normal, FALSE );
	shader->light_intens = MAX( 0.0f, vec3_dot( shader->vert_nrm, shader->light_pos ) );
	shader->var_intens.data[index] = MAX( 0.0f, vec4_dot( ( vert->normal ), vec4_nrm( vec4_sub( vec3_toVec4( shader->light_pos ), vert->position ) ) ) );
	return vec4_byMat4( vert->position, _mvp );
}

color_t Shader_Fragment( shader_t *shader, obj_model_t *model, vec3 bary ) {
	float intens = vec3_dot( shader->var_intens, bary );
	vec2  uv     = vec4_toVec2( vec4_byMat4( vec3_toVec4( bary ), shader->var_uv ) );
	return Color_Intensity( Bitmap_GetPixelUV( model->diffmap->bitmap, uv ), intens );
	return Bitmap_GetPixelUV( model->diffmap->bitmap, uv );
}

void Shader_SetLight( shader_t *shader, vec3 lpos, vec3 lcolor ) {
	shader->light_pos = lpos;
	shader->light_color = lcolor;
}
