#include "srdefs.h"
#include "raster.h"

extern renderer_t *renderer_;



static float EdgeFunc( vec3 a, vec3 b, vec3 c ) {
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

static float TriangleAreaTimesTwo( vertex_t *a, vertex_t *b, vertex_t *c ) {
    float x1 = b->position.x - a->position.x;
    float y1 = b->position.y - a->position.y;
    float x2 = c->position.x - a->position.x;
    float y2 = c->position.y - a->position.y;
    return ( x1 * y2 - x2 * y1 );
}

void Raster_DrawLine(vec4 v0, vec4 v1, SDL_Surface *image, color_t color) {
    mat4 sst = mat4_screen ( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 );
    
    vec2i p0 = vec4_toVec2i( vec4_pdiv( vec4_byMat4( v0, sst ) ) );
    vec2i p1 = vec4_toVec2i( vec4_pdiv( vec4_byMat4( v1, sst ) ) );

    int x0 = p0.x;
    int y0 = p0.y;
    int x1 = p1.x;
    int y1 = p1.y;

    int deltax = abs( x1 - x0 );
    int deltay = abs( y1 - y0 );
    int signx = x0 < x1 ? 1 : -1;
    int signy = y0 < y1 ? 1 : -1;
    int error2, error = ( deltax > deltay ? deltax : -deltay ) / 2;

    while(1) {
        if( x0 >= 0 && y0 >= 0 && x0 < image->w && y0 < image->h )
            renderer_->PutPixel( image, x0, y0, Color_ToUInt32( color ) );
        if( x0 == x1 && y0 == y1 )
            break;

        error2 = error;
        if ( error2 >- deltax ){
            error -= deltay;
            x0    += signx;
        }
        if ( error2 <  deltay ){
            error += deltax;
            y0    += signy;
        }
    }
}

void Raster_DrawTriangle( vec4 *v, SDL_Surface *Surface, obj_model_t *model, shader_t *shader ) {
	float w0, w1, w2, z, area;
	int i, x ,y, srcX, srcY;
	
	mat4 sst    = mat4_screen( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 );
	vec3 p;
	vec3 pts[3] = {vec4_toVec3( vec4_byMat4(v[0], sst), TRUE ),
				   vec4_toVec3( vec4_byMat4(v[1], sst), TRUE ),
				   vec4_toVec3( vec4_byMat4(v[2], sst), TRUE )};
				   
	vec2i bboxmin = vec2i_create( 0, 0 );
	vec2i bboxmax = vec2i_create( WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 );
	vec2i clamp   = vec2i_create( WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 );
	
	area = EdgeFunc( pts[0], pts[1], pts[2] );
	
	bboxmax.x = MIN( MAX( pts[0].x, MAX( pts[1].x, pts[2].x) ), clamp.x );
	bboxmin.x = MAX( MIN( pts[0].x, MIN( pts[1].x, pts[2].x) ), 0 );
	bboxmax.y = MIN( MAX( pts[0].y, MAX( pts[1].y, pts[2].y) ), clamp.y );
	bboxmin.y = MAX( MIN( pts[0].y, MIN( pts[1].y, pts[2].y) ), 0 );

	for( x = bboxmin.x; x <= bboxmax.x; x++ ) {
		for( y = bboxmin.y; y <= bboxmax.y; y++ ) {
			int index = y * WINDOW_WIDTH + x;
			p = vec3_create( x, y, 0.0f );
			float w0 = EdgeFunc( pts[1], pts[2], p );
			float w1 = EdgeFunc( pts[2], pts[0], p );
			float w2 = EdgeFunc( pts[0], pts[1], p );
			
			if( w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f ) {
				w0 /= area;
				w1 /= area;
				w2 /= area;
				z = 1.0f / ( pts[0].z * w0 + pts[1].z * w1 + pts[2].z * w2 );
				
				if( z < renderer_->z_Buffer[index] ) {
					renderer_->z_Buffer[index] = z;
				
					renderer_->PutPixel( Surface, p.x, p.y, Color_ToUInt32( Shader_Fragment( shader, model, vec3_create(w0, w1, w2) ) ) );
				}
			}
		}
	}
}
