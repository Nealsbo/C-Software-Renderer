#ifndef OBJLOADER_H_INCLUDED
#define OBJLOADER_H_INCLUDED

typedef struct {
    vec4 pos[3];
    vec4 norm[3];
    vec2 uv[3];
    color_t color;
} triangle_t;

typedef struct {
    bitmap_t *bitmap;
} texture_t;

typedef struct {
    int   vcount;
    vec3i *indexes;
} obj_face_t;

typedef struct {
    vec4 position;
    vec4 normal;
    vec2 uv;
} vertex_t;

typedef struct {
    char         name[MAX_NAME_LENGTH];

    vec3         *vertices;
    obj_face_t   *faces;
    vec3         *normals;
    vec2         *textcoords;

    int          vertCount;
    int          coordCount;
    int          normCount;
    int          trisCount;

    vec3         position;
    vec3         rotation;
    vec3         scale;

    color_t      baseColor;

    texture_t     *diffmap;

    int          *indexes;
} obj_model_t;

vertex_t Vertex_Init     ( vec4 pos, vec4 norm, vec2 uv );
void     Vertex_Transform( vertex_t vert, mat4 tf );
vertex_t Vertex_Lerp     ( vertex_t v, float value );
float    Vertex_GetPositionElement( vertex_t v, int index );

triangle_t  *CreateTriangle();

obj_face_t   Face_Create(int n, vec3i *v);
void         Face_Print (obj_face_t f);
void         Face_Printp(obj_face_t *f);

//###########################
//###   Model functions   ###
//###########################

//TODO: Refactoring

obj_model_t *Model_Create            ( const char *model_name );
obj_model_t *Model_CreateBaseTriangle( const char *model_name );
obj_model_t *Model_CreateBasePlane   ( const char *model_name );
obj_model_t *Model_CreateBaseBox     ( const char *model_name );

obj_model_t *Model_LoadOBJ           ( const char *file_name );
void         Model_Test              ( obj_model_t *model );

void         Model_SetPosition       ( obj_model_t *model, vec3 pos );
void         Model_SetRotation       ( obj_model_t *model, vec3 rot );
void         Model_SetScale          ( obj_model_t *model, vec3 scale );
void         Model_SetBaseColor      ( obj_model_t *model, color_t color );
void         Model_Rename            ( obj_model_t *model, char *new_name );

void         Model_AddPosition       ( obj_model_t *model, vec3 pos );
void         Model_AddRotation       ( obj_model_t *model, vec3 rot );

char        *Model_GetName           ( obj_model_t *model );
vec3         Model_GetPosition       ( obj_model_t *model );
vec3         Model_GetRotation       ( obj_model_t *model );
vec3         Model_GetScale          ( obj_model_t *model );
color_t      Model_GetBaseColor      ( obj_model_t *model );
int          Model_GetVertCount      ( obj_model_t *model );
int          Model_GetTrisCount      ( obj_model_t *model );
texture_t   *Model_GetTexture        ( obj_model_t *model );

void         Model_Free              ( obj_model_t *model );
#endif // OBJLOADER_H_INCLUDED
