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
	vec3 fragPos, normal, ambient, diffuse, light_dir, view_dir, refl_dir, specular, res;
	vec2 uv;
	float diff, spec;
	//vec2 uv      = vec4_toVec2( vec4_byMat4( vec3_toVec4( bary ), s->var_uv ) );
	uv      = vec2_create( bary.x*s->uvs[0].x + bary.y*s->uvs[1].x + bary.z*s->uvs[2].x,
						   bary.x*s->uvs[0].y + bary.y*s->uvs[1].y + bary.z*s->uvs[2].y );
                                
	if( !s->isLit ) {
		return Bitmap_GetPixelUV( model->diffmap->bitmap, uv );
	}
	
	float ambientStr = 0.1f, specularStr = 1.0f;
	//vec3 fragPos = vec3_nrm( vec3_byMat4( bary, s->var_pos ) );
	//vec3 normal  = vec3_nrm( vec3_byMat4( bary , s->var_nrm ) );
	fragPos = vec3_nrm( vec3_create( bary.x*s->pos[0].x + bary.y*s->pos[1].x + bary.z*s->pos[2].x,
									 bary.x*s->pos[0].y + bary.y*s->pos[1].y + bary.z*s->pos[2].y,
									 bary.x*s->pos[0].z + bary.y*s->pos[1].z + bary.z*s->pos[2].z ) );
                                          
	normal  = vec3_nrm( vec3_create( bary.x*s->normals[0].x + bary.y*s->normals[1].x + bary.z*s->normals[2].x + s->normals[3].x,
									 bary.x*s->normals[0].y + bary.y*s->normals[1].y + bary.z*s->normals[2].y + s->normals[3].y,
									 bary.x*s->normals[0].z + bary.y*s->normals[1].z + bary.z*s->normals[2].z + s->normals[3].z) );
	
	if( model->normalmap != NULL ) {
		vec3 texN = vec3_sub( vec3_mlt( Color_ToVec3( Bitmap_GetPixelUV( model->normalmap->bitmap, uv ) ), 2.0f ), vec3_create( 1.0f, 1.0f, 1.0f ) ); // normal * 2 - 1
		if( s->isNormalInverted ) // if required to invert normal map
			normal = vec3_nrm( vec3_add( vec3_create( -texN.x, -texN.y, texN.z ), normal ) );
		else
			normal = vec3_nrm( vec3_add( texN, normal ) );
	}

	// ambient light
	ambient    = vec3_mlt( s->ambient_color, ambientStr );
	
	// diffuse light
	light_dir = vec3_nrm( vec3_sub( s->light_pos, fragPos ) );
	diff      = MAX( vec3_dot( normal, light_dir ), 0.0f );
	diffuse   = vec3_mlt( s->light_color, diff );
	
	// specular light
	view_dir  = vec3_nrm( vec3_sub( s->cam_pos, fragPos ) );
	refl_dir  = vec3_nrm( vec3_refl( vec3_neg( light_dir ), normal ) );
	spec      = powf( MAX( vec3_dot( view_dir, refl_dir ), 0.0f ), 32.0f );
	specular  = vec3_mlt( s->light_color, specularStr * spec );
	
	res       = vec3_add( vec3_add( vec3_mlt( diffuse, 0.5f ), ambient ), specular );
	
	return Color_IntensityVec3( Bitmap_GetPixelUV( model->diffmap->bitmap, uv ), res );
}

void Shader_UseLight( shader_t *shader, int to_use ) {
	shader->isLit = to_use;
}

void Shader_InvertNormal( shader_t *shader, int to_inv ) {
	shader->isNormalInverted = to_inv;
}

void Shader_SetMatrices( shader_t *shader, mat4 model, mat4 view, mat4 mvp ) {
	mat4 tmp;
	mat4_copy( model, shader->m );
	mat4_copy( view, shader->v );
	
	mat4_inv( shader->m, tmp );
	mat4_tsp( tmp, shader->m_it );
	
	mat4_copy( mvp, shader->mvp );
}

void Shader_SetCamera( shader_t *shader, vec3 cam_pos ) {
	shader->cam_pos = cam_pos;
}

void Shader_SetLight( shader_t *shader, vec3 dlpos, vec3 dlcolor, vec3 alcolor ) {
	shader->light_pos = dlpos;
	shader->light_color = dlcolor;
	shader->ambient_color = alcolor;
}
