#include "srdefs.h"
#include "shader.h"

void Shader_Vertex( shader_t *shader, vertex_t *vert, mat4 mvp ) {
	shader->verts    = vec4_toVec3( vert->position, FALSE );
	shader->vert_nrm = vec4_toVec3( vert->normal, FALSE );
	shader->vert_uv  = vert->uv;
}

color_t Shader_Fragment( shader_t *shader, obj_model_t *model, vec2 uv, vec3 normal ) {
	color_t fragColor = { 255, 255, 255, 255 };
	return fragColor;
}
