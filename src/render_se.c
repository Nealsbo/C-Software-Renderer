#include "srdefs.h"
#include "render_se.h"

extern renderer_t *renderer_;

static void ScanTriangle( vertex_t *miny, vertex_t *midy, vertex_t *maxy, int handedness, SDL_Surface *Surface, bitmap_t *texture );
static void ScanEdges(edge_t *a, edge_t *b, int handedness, SDL_Surface *Surface, bitmap_t *texture);
static void DrawScanLine(edge_t *left, edge_t *right, int j, SDL_Surface *Surface, bitmap_t *texture);

//############################
//###   Raster functions   ###
//############################

static edge_t Edge_Init(gradient_t grad, vertex_t *miny, vertex_t *maxy, int miny_index){
    edge_t edge;

    edge.yStart = (int)ceilf(miny->position.y);
    edge.yEnd   = (int)ceilf(maxy->position.y);

    float yDist = maxy->position.y - miny->position.y;
    float xDist = maxy->position.x - miny->position.x;

    float yPrestep = edge.yStart - miny->position.y;
    edge.xStep   = xDist / yDist;
    edge.x       = miny->position.x + yPrestep * edge.xStep;
    float xPrestep = edge.x - miny->position.x;

    edge.texCoordX     = grad.texCoordX[miny_index] + grad.texCoordXXStep * xPrestep + grad.texCoordXYStep * yPrestep;
    edge.texCoordXStep = grad.texCoordXYStep + grad.texCoordXXStep * edge.xStep;
    edge.texCoordY     = grad.texCoordY[miny_index] + grad.texCoordYXStep * xPrestep + grad.texCoordYYStep * yPrestep;
    edge.texCoordYStep = grad.texCoordYYStep + grad.texCoordYXStep * edge.xStep;
    edge.oneOverZ      = grad.oneOverZ[miny_index]  + grad.oneOverZXStep  * xPrestep + grad.oneOverZYStep  * yPrestep;
    edge.oneOverZStep  = grad.oneOverZYStep  + grad.oneOverZXStep  * edge.xStep;
    edge.depth         = grad.depth[miny_index]     + grad.depthXStep     * xPrestep + grad.depthYStep     * yPrestep;
    edge.depthStep     = grad.depthYStep     + grad.depthXStep     * edge.xStep;
    edge.light         = grad.light[miny_index]     + grad.lightXStep     * xPrestep + grad.lightYStep     * yPrestep;
    edge.lightStep     = grad.lightYStep     + grad.lightXStep     * edge.xStep;

    return edge;
}

static void Edge_Step(edge_t *edge){
    edge->x         += edge->xStep;
    edge->texCoordX += edge->texCoordXStep;
    edge->texCoordY += edge->texCoordYStep;
    edge->oneOverZ  += edge->oneOverZStep;
    edge->depth     += edge->depthStep;
    edge->light     += edge->lightStep;
}

static gradient_t Gradient_Init(vertex_t *miny, vertex_t *midy, vertex_t *maxy){
    gradient_t grad;
    vec4 light = renderer_->scene->dummLight;
    float oneOverdX = 1.0f /
            (((midy->position.x - maxy->position.x) * (miny->position.y - maxy->position.y)) -
             ((miny->position.x - maxy->position.x) * (midy->position.y - maxy->position.y)));

    float oneOverdY = -oneOverdX;

    grad.depth[0] = miny->position.z;
    grad.depth[1] = midy->position.z;
    grad.depth[2] = maxy->position.z;
    
    grad.light[0] = clamp( vec3_dot( vec4_toVec3(miny->normal, TRUE), vec4_toVec3(light, FALSE)), 0.0f, 1.0f ) * 0.8f + 0.2f;
    grad.light[1] = clamp( vec3_dot( vec4_toVec3(midy->normal, TRUE), vec4_toVec3(light, FALSE)), 0.0f, 1.0f ) * 0.8f + 0.2f;
    grad.light[2] = clamp( vec3_dot( vec4_toVec3(maxy->normal, TRUE), vec4_toVec3(light, FALSE)), 0.0f, 1.0f ) * 0.8f + 0.2f;

    grad.oneOverZ[0] = 1.0f / miny->position.w;
    grad.oneOverZ[1] = 1.0f / midy->position.w;
    grad.oneOverZ[2] = 1.0f / maxy->position.w;

    grad.texCoordX[0] = miny->uv.x * grad.oneOverZ[0];
    grad.texCoordX[1] = midy->uv.x * grad.oneOverZ[1];
    grad.texCoordX[2] = maxy->uv.x * grad.oneOverZ[2];

    grad.texCoordY[0] = miny->uv.y * grad.oneOverZ[0];
    grad.texCoordY[1] = midy->uv.y * grad.oneOverZ[1];
    grad.texCoordY[2] = maxy->uv.y * grad.oneOverZ[2];

    grad.texCoordXXStep = Gradient_CalcXStep( grad.texCoordX, miny, midy, maxy, oneOverdX );
    grad.texCoordXYStep = Gradient_CalcYStep( grad.texCoordX, miny, midy, maxy, oneOverdY );
    grad.texCoordYXStep = Gradient_CalcXStep( grad.texCoordY, miny, midy, maxy, oneOverdX );
    grad.texCoordYYStep = Gradient_CalcYStep( grad.texCoordY, miny, midy, maxy, oneOverdY );
    grad.oneOverZXStep  = Gradient_CalcXStep( grad.oneOverZ,  miny, midy, maxy, oneOverdX );
    grad.oneOverZYStep  = Gradient_CalcYStep( grad.oneOverZ,  miny, midy, maxy, oneOverdY );
    grad.depthXStep     = Gradient_CalcXStep( grad.depth,     miny, midy, maxy, oneOverdX );
    grad.depthYStep     = Gradient_CalcYStep( grad.depth,     miny, midy, maxy, oneOverdY );
    grad.lightXStep     = Gradient_CalcXStep( grad.light,     miny, midy, maxy, oneOverdX );
    grad.lightYStep     = Gradient_CalcYStep( grad.light,     miny, midy, maxy, oneOverdY );
    return grad;
}

static float Gradient_CalcXStep( float *values, vertex_t *miny, vertex_t *midy, vertex_t *maxy, float oneOverdX ){
    return (((values[1] - values[2]) * (miny->position.y - maxy->position.y)) -
            ((values[0] - values[2]) * (midy->position.y - maxy->position.y))) * oneOverdX;
}

static float Gradient_CalcYStep( float *values, vertex_t *miny, vertex_t *midy, vertex_t *maxy, float oneOverdY ){
    return (((values[1] - values[2]) * (miny->position.x - maxy->position.x)) -
            ((values[0] - values[2]) * (midy->position.x - maxy->position.x))) * oneOverdY;
}

static float TriangleAreaTimesTwo( vertex_t *a, vertex_t *b, vertex_t *c ){
    float x1 = b->position.x - a->position.x;
    float y1 = b->position.y - a->position.y;
    float x2 = c->position.x - a->position.x;
    float y2 = c->position.y - a->position.y;
    return ( x1 * y2 - x2 * y1 );
}

static void Render_DrawLine(vertex_t v0, vertex_t v1, SDL_Surface *image, color_t color) {
    mat4 sst = mat4_screen ( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 );
    vec2i p0 = vec4_toVec2i(vec4_pdiv(vec4_byMat4(v0.position, sst)));
    vec2i p1 = vec4_toVec2i(vec4_pdiv(vec4_byMat4(v1.position, sst)));

    int x0 = p0.x, y0 = p0.y, x1 = p1.x, y1 = p1.y;

    int deltax = abs( x1 - x0 ), signx = x0 < x1 ? 1 : -1;
    int deltay = abs( y1 - y0 ), signy = y0 < y1 ? 1 : -1;
    int error2, error = ( deltax > deltay ? deltax : -deltay) / 2;

    while(1) {
        if( x0 >= 0 && y0 >= 0 && x0 < image->w && y0 < image->h )
            renderer_->PutPixel( image, x0, y0, Color_ToUInt32(color) );
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

static void Render_DrawTriangle( vertex_t *v, SDL_Surface *Surface, obj_model_t *model ) {
	vertex_t *miny, *midy, *maxy;
	miny = &v[0];
	midy = &v[1];
	maxy = &v[2];
    mat4 sst       = mat4_screen( WINDOW_WIDTH/2, WINDOW_HEIGHT/2 );
    miny->position = vec4_pdiv(vec4_byMat4( miny->position, sst ));
    midy->position = vec4_pdiv(vec4_byMat4( midy->position, sst ));
    maxy->position = vec4_pdiv(vec4_byMat4( maxy->position, sst ));

    if( TriangleAreaTimesTwo( miny, maxy, midy ) >= 0)
        return;

    if( maxy->position.y < midy->position.y )
        SWAP(maxy, midy, vertex_t *);

    if( midy->position.y < miny->position.y )
        SWAP(midy, miny, vertex_t *);

    if( maxy->position.y < midy->position.y )
        SWAP(maxy, midy, vertex_t *);

    ScanTriangle( miny, midy, maxy, TriangleAreaTimesTwo(miny, maxy, midy) >= 0, Surface, model->diffmap->bitmap );
}

static void ScanTriangle( vertex_t *miny, vertex_t *midy, vertex_t *maxy, int handedness, SDL_Surface *Surface, bitmap_t *texture ){
        gradient_t grad       = Gradient_Init( miny, midy, maxy );

        edge_t topToBottom    = Edge_Init( grad, miny, maxy, 0) ;
        edge_t topToMiddle    = Edge_Init( grad, miny, midy, 0 );
        edge_t middleToBottom = Edge_Init( grad, midy, maxy, 1 );

        ScanEdges( &topToBottom, &topToMiddle, handedness, Surface, texture );
        ScanEdges( &topToBottom, &middleToBottom, handedness, Surface, texture );
}

static void ScanEdges(edge_t *a, edge_t *b, int handedness, SDL_Surface *Surface, bitmap_t *texture){
        int j;
        edge_t *left = a;
        edge_t *right = b;
        if(handedness)
            SWAP(left, right, edge_t *);

        int yStart = b->yStart;
        int yEnd   = b->yEnd;

        for(j = yStart; j < yEnd; j++) {
            DrawScanLine(left, right, j, Surface, texture);
            Edge_Step(left);
            Edge_Step(right);
        }
}

static void DrawScanLine(edge_t *left, edge_t *right, int j, SDL_Surface *Surface, bitmap_t *texture){
    int i;
    int xMin = (int)ceilf(left->x);
    int xMax = (int)ceilf(right->x);

    float xPrestep = xMin - left->x;
    float xDist    = right->x - left->x;

    float texCoordXXStep = (right->texCoordX - left->texCoordX)/xDist;
    float texCoordYXStep = (right->texCoordY - left->texCoordY)/xDist;
    float oneOverZXStep  = (right->oneOverZ  - left->oneOverZ )/xDist;
    float depthXStep     = (right->depth     - left->depth    )/xDist;
    float lightXStep     = (right->light     - left->light    )/xDist;

    float texCoordX = left->texCoordX + texCoordXXStep * xPrestep;
    float texCoordY = left->texCoordY + texCoordYXStep * xPrestep;
    float oneOverZ  = left->oneOverZ  + oneOverZXStep  * xPrestep;
    float depth     = left->depth     + depthXStep     * xPrestep;
    float light     = left->light     + lightXStep     * xPrestep;

    for( i = xMin; i < xMax; i++ ){

        int index = j * WINDOW_WIDTH + i;
        if( depth < renderer_->z_Buffer[index] ){
            renderer_->z_Buffer[index] = depth;
            float z = 1.0f / oneOverZ;
            float lightStr = light;
            unsigned int srcX = (unsigned int)((texCoordX * z) * (float)(texture->width  - 1) + 0.5f);
            unsigned int srcY = (unsigned int)((texCoordY * z) * (float)(texture->height - 1) + 0.5f);
            renderer_->PutPixel(Surface, i, j, Color_ToUInt32(Color_Intensity(Bitmap_GetPixel(texture, srcX, srcY), lightStr)));
        }
        oneOverZ  += oneOverZXStep;
        texCoordX += texCoordXXStep;
        texCoordY += texCoordYXStep;
        depth     += depthXStep;
        light     += lightXStep;
    }
}

void Render_SE_DrawObject(scene_t *scene, renderer_t *renderer, obj_model_t *model, SDL_Surface *Surface ){
    int i, j;
    obj_face_t face;
    vertex_t vertex[3];
    
    renderer_ = renderer;
    camera_t *cam = scene->mainCamera;

    mat4 worldToCamera = mat4_lookAt(cam->position, cam->front, vec3_create(0.0, 1.0, 0.0));

    mat4 translation   = mat4_translate( model->position.x, model->position.y, model->position.z );
    mat4 rotation      = mat4_rotation( model->rotation.x, model->rotation.y, model->rotation.z );
    mat4 scale         = mat4_scale( model->scale.x, model->scale.y, model->scale.z );
    mat4 modelMat      = mat4_mlt( mat4_mlt(translation, rotation), scale );

    mat4 cameraMat     = mat4_mlt( worldToCamera, modelMat );
    mat4 viewMat       = mat4_mlt( scene->mainCamera->projection, cameraMat );

    for(i = 0; i < model->trisCount; i++){
        face = model->faces[i];
        for(j = 0; j < face.vcount; j++){
            vertex[j] = Vertex_Init( vec4_byMat4(vec3_toVec4(model->vertices[face.indexes[j].x - 1]), viewMat),
                                     vec4_byMat4(vec3_toVec4(model->normals[face.indexes[j].z - 1]), rotation),
                                     model->textcoords[face.indexes[j].y - 1] );
        }

        switch(renderer->flagState){
            case RENDER_STATE_WIREFRAME:
                Render_DrawLine( vertex[0], vertex[1], Surface, model->baseColor );
                Render_DrawLine( vertex[1], vertex[2], Surface, model->baseColor );
                Render_DrawLine( vertex[0], vertex[2], Surface, model->baseColor );
                break;

            case RENDER_STATE_LIT:
                Render_DrawTriangle( vertex, Surface, model );
                break;

            default:
                break;
        }
    }
}
