#include "srdefs.h"
#include "shader.h"

vec4 Shader_Vertex( shader_t *shader, vertex_t *vert, mat4 _mvp, int index ) {
	shader->mvp = _mvp;
	shader->vert_nrm = vec4_toVec3( vert->normal, FALSE );
	shader->vert_uv  = vert->uv;
	shader->light_intens = MAX( 0.0f, vec3_dot( shader->vert_nrm, shader->light_pos ) );
	shader->var_intens.data[index] = MAX( 0.0f, vec4_dot( vert->normal, vec4_sub( vec3_toVec4( shader->light_pos ), vert->position ) ) );
	return vec4_byMat4( vert->position, _mvp );
}

color_t Shader_Fragment( shader_t *shader, obj_model_t *model, vec3 bary ) {
	return Color_Gray( (unsigned char)( 255 * vec3_dot( shader->var_intens, bary ) ) );
}

void Shader_SetLight( shader_t *shader, vec3 lpos, vec3 lcolor ) {
	shader->light_pos = lpos;
	shader->light_color = lcolor;
}
