#ifndef SHADER_H_INCLUDE
#define SHADER_H_INCLUDE

#include "srdefs.h"

typedef struct {
	mat4 m, v, m_it, mvp;
	//mat4 var_uv, var_nrm, var_pos;
	vec4 pos[3], normals[3];
	vec3 light_dir, refl_dir, cam_pos, light_pos, light_color, ambient_color;
	vec3 ambient, diffuse, specular;
	vec2 uvs[3];
	float light_intens;
	int isLit, isNormalInverted;
	
} shader_t;

vec4    Shader_Vertex      ( shader_t *shader, vertex_t *vert, int index );
color_t Shader_Fragment    ( shader_t *shader, obj_model_t *model, vec3 bary );

void    Shader_UseLight    ( shader_t *shader, int to_use );
void    Shader_InvertNormal( shader_t *shader, int to_inv );
void    Shader_SetMatrices ( shader_t *shader, mat4 model, mat4 view, mat4 mvp );
void    Shader_SetCamera   ( shader_t *shader, vec3 cam_pos );
void    Shader_SetLight    ( shader_t *shader, vec3 dlpos, vec3 dlcolor, vec3 alcolor );

#endif // SHADER_H_INCLUDE
