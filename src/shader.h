#ifndef SHADER_H_INCLUDE
#define SHADER_H_INCLUDE

typedef struct {
	vec3 verts;
	vec3 vert_nrm;
	vec2 vert_uv;
} shader_t;

void    Shader_Vertex  ( shader_t *shader, vertex_t *vert, mat4 mvp );
color_t Shader_Fragment( shader_t *shader, obj_model_t *model, vec2 uv, vec3 normal );

#endif // SHADER_H_INCLUDE
