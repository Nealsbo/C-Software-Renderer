#ifndef SHADER_H_INCLUDE
#define SHADER_H_INCLUDE

#include "srdefs.h"

typedef struct {
	mat4 mvp;
	mat4 mvp_inv;
	mat3 var_uv;
	vec3 var_intens;
	vec3 vert_nrm;
	vec2 vert_uv;
	vec3 light_pos;
	vec3 light_color;
	float light_intens;
} shader_t;

vec4    Shader_Vertex  ( shader_t *shader, vertex_t *vert, mat4 _mvp, int index );
color_t Shader_Fragment( shader_t *shader, obj_model_t *model, vec3 bary );

void    Shader_SetLight( shader_t *shader, vec3 lpos, vec3 lcolor );

#endif // SHADER_H_INCLUDE
