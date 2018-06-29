#include "srdefs.h"
#include "objloader.h"

vertex_t Vertex_Init(vec4 pos, vec3 norm, vec2 uv){
    vertex_t vert;
    vert.position = (vec4){pos.x, pos.y, pos.z, pos.w};
    vert.normal   = (vec3){norm.x, norm.y, norm.z};
    vert.uv       = (vec2){uv.x, uv.y};
    return vert;
}

void Vertex_Transform(vertex_t vert, mat4 tf){
    vert.position = vec4_byMat4(vert.position, tf);
}

vertex_t Vertex_Lerp( vertex_t v, float value ){
    vertex_t vert;
    vert.position = vec4_lerp( vert.position, v.position, value );
    vert.uv       = vec2_lerp( vert.uv, v.uv, value );
    return vert;
}

float Vertex_GetPositionElement( vertex_t v, int index ){
    switch( index ){
    case 0:
        return v.position.x;
    case 1:
        return v.position.y;
    case 2:
        return v.position.z;
    case 3:
        return v.position.w;
    default:
        return 0;
    }
}

triangle_t *CreateTriangle(){
    triangle_t *tris = malloc(sizeof(triangle_t));
    return tris;
}

obj_face_t Face_Create(int n, vec3i *v){
    int i;
    obj_face_t f;
    f.vcount = n;
    f.indexes = v;
    for(i = 0; i < n; i++)
        f.indexes[i] = v[i];
    return f;
}

void Face_Print(obj_face_t f){
    int i;
    printf("Face:");
    for(i = 0; i <f.vcount; i++)
        printf(" %d/%d/%d", f.indexes[i].x, f.indexes[i].y, f.indexes[i].z );
    printf("\n");
}

void Face_Printp(obj_face_t *f){
    int i;
    printf("Face:");
    for(i = 0; i <f->vcount; i++)
        printf(" %d/%d/%d", f->indexes[i].x, f->indexes[i].y, f->indexes[i].z );
    printf("\n");
}

obj_model_t *Model_Create( const char *model_name){
    obj_model_t *model = (obj_model_t *)malloc(sizeof(obj_model_t));
    if(model == NULL){
        printf("Error: Cannot create model object '%s'\n", model_name);
        return NULL;
    }

    memcpy(model->name, model_name, MAX_NAME_LENGTH);

    model->vertCount   = 0;                                             //Allocation prepared data for model, box in that case
    model->trisCount   = 0;
    model->position    = vec3_create(0.1, 2.3, 4.5);
    model->rotation    = vec3_create(6.7, 8.9, 0.1);
    model->scale       = vec3_create(1.3, 1.6, 1.9);
    model->baseColor   = Color_Init(63, 127, 191, 255);
    model->diffmap     = NULL;
    model->vertices    = NULL;
    model->faces       = NULL;
    model->normals     = NULL;
    model->textcoords  = NULL;

    return model;
}

obj_model_t *Model_CreateBaseTriangle( const char *model_name ){
    int i;

    obj_model_t *model = (obj_model_t *)malloc(sizeof(obj_model_t));
    if(model == NULL){
        printf("Error: Cannot create model object '%s'\n", model_name);
        return NULL;
    }

    memcpy(model->name, model_name, MAX_NAME_LENGTH);                   //Set object name by argument

    model->vertCount   = 3;                                             //Allocation prepared data for model, box in that case
    model->trisCount   = 1;

    float v_arr[3][3]  = {{-1.0, -1.0, 0.0},
                          { 0.0,  1.0, 0.0},
                          { 1.0, -1.0, 0.0}};

    float vn_arr[3][3] = {{ 0.0, 0.0, 1.0},
                          { 0.0, 0.0, 1.0},
                          { 0.0, 0.0, 1.0}};

    float vt_arr[3][2] = {{ 0.0,  0.0},
                          { 0.5,  1.0},
                          { 1.0,  0.0}};

    int   f_arr[1][9]  = {{ 1, 1, 1, 2, 2, 2, 3, 3, 3}};

    model->position    = vec3_create(0.0, 0.0, 0.0);
    model->rotation    = vec3_create(0.0, 0.0, 0.0);
    model->scale       = vec3_create(1.0, 1.0, 1.0);
    model->baseColor   = Color_Init(0, 245, 116, 255);
    model->diffmap     = NULL;

    model->vertices    = malloc(sizeof(vec3) * 3);
    if(model->vertices == NULL){
        printf("Error: Malloc vertices error'%s'\n", model->name);
        Model_Free(model);
        return NULL;
    }
    for(i = 0; i < 3; i++){
        model->vertices[i] = vec3_create(v_arr[i][0], v_arr[i][1], v_arr[i][2]);
    }

    model->normals     = (vec3 *)malloc(sizeof(vec3) * 3);
    for(i = 0; i < 3; i++){
        model->normals[i] = vec3_create(vn_arr[i][0], vn_arr[i][1], vn_arr[i][2]);
    }

    model->textcoords  = (vec2*)malloc(sizeof(vec2) * 3);
    for(i = 0; i < 3; i++){
        model->textcoords[i] = vec2_create(vt_arr[i][0], vt_arr[i][1]);
    }

    model->faces  = (obj_face_t *)malloc(sizeof(obj_face_t));
    model->faces[0]  = Face_Create(3, vec3i_arrayToVec3ip(9, f_arr[0] ));

    model->diffmap = (texture_t *)malloc(sizeof(texture_t));
    model->diffmap->bitmap = Bitmap_LoadPPM6("./assets/texture.ppm");

    return model;
}

obj_model_t *Model_CreateBasePlane( const char *model_name ){
    int i;

    obj_model_t *model = (obj_model_t *)malloc(sizeof(obj_model_t));
    if(model == NULL){
        printf("Error: Cannot create model object '%s'\n", model_name);
        return NULL;
    }

    memcpy(model->name, model_name, MAX_NAME_LENGTH);                   //Set object name by argument

    model->vertCount   = 4;                                             //Allocation prepared data for model, box in that case
    model->trisCount   = 2;

    float v_arr[4][3]  = {{-1.0, -1.0, 0.0},
                          {-1.0,  1.0, 0.0},
                          { 1.0,  1.0, 0.0},
                          { 1.0, -1.0, 0.0}};

    float vn_arr[4][3] = {{ 0.0, 0.0, 1.0},
                          { 0.0, 0.0, 1.0},
                          { 0.0, 0.0, 1.0},
                          { 0.0, 0.0, 1.0}};

    float vt_arr[4][2] = {{ 0.0,  0.0},
                          { 0.0,  1.0},
                          { 1.0,  1.0},
                          { 1.0,  0.0}};

    int   f_arr[2][9]  = {{ 1, 1, 1, 2, 2, 2, 3, 3, 3},
                          { 1, 1, 1, 3, 3, 3, 4, 4, 4}};

    model->position    = vec3_create(0.0, 0.0, 0.0);
    model->rotation    = vec3_create(0.0, 0.0, 0.0);
    model->scale       = vec3_create(1.0, 1.0, 1.0);
    model->baseColor   = Color_Init(0, 245, 116, 255);
    model->diffmap     = NULL;

    model->vertices    = malloc(sizeof(vec3) * 4);
    if(model->vertices == NULL){
        printf("Error: Malloc vertices error'%s'\n", model->name);
        Model_Free(model);
        return NULL;
    }
    for(i = 0; i < 4; i++){
        model->vertices[i] = vec3_create(v_arr[i][0], v_arr[i][1], v_arr[i][2]);
    }

    model->normals     = (vec3 *)malloc(sizeof(vec3) * 4);
    for(i = 0; i < 4; i++){
        model->normals[i] = vec3_create(vn_arr[i][0], vn_arr[i][1], vn_arr[i][2]);
    }

    model->textcoords  = (vec2*)malloc(sizeof(vec2) * 4);
    for(i = 0; i < 4; i++){
        model->textcoords[i] = vec2_create(vt_arr[i][0], vt_arr[i][1]);
    }

    model->faces  = (obj_face_t *)malloc(sizeof(obj_face_t) * 2);
    model->faces[0]  = Face_Create(3, vec3i_arrayToVec3ip(9, f_arr[0] ));
    model->faces[1]  = Face_Create(3, vec3i_arrayToVec3ip(9, f_arr[1] ));

    model->diffmap = (texture_t *)malloc(sizeof(texture_t));
    model->diffmap->bitmap = Bitmap_LoadPPM6("./assets/texture.ppm");

    return model;
}

obj_model_t *Model_CreateBaseBox( const char *model_name){
    int i;
    obj_model_t *model = (obj_model_t *)malloc(sizeof(obj_model_t));
    if(model == NULL){
        printf("Error: Cannot create model object '%s'\n", model_name);
        return NULL;
    }

    memcpy(model->name, model_name, MAX_NAME_LENGTH);                   //Set object name by argument

    model->vertCount   = 8;                                             //Allocation prepared data for model, box in that case
    model->trisCount   = 12;

    float    v_arr[8][3]  = {{-1.0, -1.0,  1.0},                        //Vertices data for box model
                             { 1.0, -1.0,  1.0},
                             {-1.0,  1.0,  1.0},
                             { 1.0,  1.0,  1.0},
                             {-1.0,  1.0, -1.0},
                             { 1.0,  1.0, -1.0},
                             {-1.0, -1.0, -1.0},
                             { 1.0, -1.0, -1.0}};

    float   vn_arr[24][3] = {{ 0.0,  0.0,  1.0},
                             { 0.0,  0.0,  1.0},
                             { 0.0,  0.0,  1.0},
                             { 0.0,  0.0,  1.0},
                             { 0.0,  1.0,  0.0},
                             { 0.0,  1.0,  0.0},
                             { 0.0,  1.0,  0.0},
                             { 0.0,  1.0,  0.0},
                             { 0.0,  0.0, -1.0},
                             { 0.0,  0.0, -1.0},
                             { 0.0,  0.0, -1.0},
                             { 0.0,  0.0, -1.0},
                             { 0.0, -1.0,  0.0},
                             { 0.0, -1.0,  0.0},
                             { 0.0, -1.0,  0.0},
                             { 0.0, -1.0,  0.0},
                             { 1.0,  0.0,  0.0},
                             { 1.0,  0.0,  0.0},
                             { 1.0,  0.0,  0.0},
                             { 1.0,  0.0,  0.0},
                             {-1.0,  0.0,  0.0},
                             {-1.0,  0.0,  0.0},
                             {-1.0,  0.0,  0.0},
                             {-1.0,  0.0,  0.0}};

    float   vt_arr[24][2] = {{ 0.0, 0.0},
                             { 1.0, 0.0},
                             { 1.0, 1.0},
                             { 0.0, 1.0},
                             {-0.0, 0.0},
                             { 1.0, 0.0},
                             { 1.0, 1.0},
                             {-0.0, 1.0},
                             {-0.0, 0.0},
                             { 1.0, 0.0},
                             { 1.0, 1.0},
                             {-0.0, 1.0},
                             { 0.0, 0.0},
                             { 1.0, 0.0},
                             { 1.0, 1.0},
                             { 0.0, 1.0},
                             { 0.0, 0.0},
                             { 1.0, 0.0},
                             { 1.0, 1.0},
                             { 0.0, 1.0},
                             { 0.0, 0.0},
                             { 1.0, 0.0},
                             { 1.0, 1.0},
                             { 0.0, 1.0}};

    int      f_arr[12][9] = {{1,  1,  1,  2,  2,  2,  3,  4,  3},       //Face index data for box model
                             {3,  4,  3,  2,  2,  2,  4,  3,  4},
                             {3,  5,  5,  4,  6,  6,  5,  8,  7},
                             {5,  8,  7,  4,  6,  6,  6,  7,  8},
                             {5,  9,  9,  6, 10, 10,  7, 12, 11},
                             {7, 12, 11,  6, 10, 10,  8, 11, 12},
                             {7, 13, 13,  8, 14, 14,  1, 16, 15},
                             {1, 16, 15,  8, 14, 14,  2, 15, 16},
                             {2, 17, 17,  8, 18, 18,  4, 20, 19},
                             {4, 20, 19,  8, 18, 18,  6, 19, 20},
                             {7, 21, 21,  1, 22, 22,  5, 24, 23},
                             {5, 24, 23,  1, 22, 22,  3, 23, 24}};

    model->position    = vec3_create(0.0, 0.0, 0.0);
    model->rotation    = vec3_create(0.0, 0.0, 0.0);
    model->scale       = vec3_create(1.0, 1.0, 1.0);
    model->baseColor   = Color_Init(255, 255, 255, 255);
    model->diffmap     = NULL;

    model->vertices    = (vec3 *)malloc(sizeof(vec3) * 8);
    for(i = 0; i < 8; i++){
        model->vertices[i] = (vec3){v_arr[i][0], v_arr[i][1], v_arr[i][2]};
    }

    model->normals     = (vec3 *)malloc(sizeof(vec3) * 24);
    for(i = 0; i < 24; i++){
        model->normals[i] = vec3_create(vn_arr[i][0], vn_arr[i][1], vn_arr[i][2]);
    }

    model->textcoords  = (vec2*)malloc(sizeof(vec2) * 24);
    for(i = 0; i < 24; i++){
        model->textcoords[i] = vec2_create(vt_arr[i][0], vt_arr[i][1]);
    }

    model->faces       = (obj_face_t *)malloc(sizeof(obj_face_t) * 12);
    for(i = 0; i < 12; i++){
        model->faces[i]  = Face_Create(3, vec3i_arrayToVec3ip(9, f_arr[i]  ));
    }

    model->diffmap = (texture_t *)malloc(sizeof(texture_t));
    model->diffmap->bitmap = Bitmap_LoadPPM6("./assets/texture.ppm");

    return model;
}

obj_model_t *Model_LoadOBJ( const char *file_name ){
	char line[MAX_NAME_LENGTH];
    FILE *fp;
    if( ( fp = fopen(file_name, "r") ) == NULL ){
        printf( "Error: Cannot open file '%s'!\n", file_name );
        return NULL;
    }
	
    obj_model_t *model = Model_Create(file_name);
    if( model == NULL ){
        printf("Error: Cannot create model for file '%s'!\n", file_name);
    }

    node_t *tmpnode = NULL;

    List *vertList  = (List *)malloc(sizeof(List));
    List *coordList = (List *)malloc(sizeof(List));
    List *normList  = (List *)malloc(sizeof(List));
    List *faceList  = (List *)malloc(sizeof(List));

    List_Init(vertList,  &vec3_printp, &free);
    List_Init(coordList, &vec2_printp, &free);
    List_Init(normList,  &vec3_printp, &free);
    List_Init(faceList,  &Face_Printp, &free);

    int fa[9] = {0};
    int i;
    int vertCount  = 0;
    int coordCount = 0;
    int normCount  = 0;
    int faceCount  = 0;

    while(!feof(fp)){
        if(fgets( line, MAX_NAME_LENGTH, fp )){
            if( line == NULL || line[0] == '#' )
                continue;

            if( line[0] == 'v' && line[1] == ' ' ){
                vertCount++;
                vec3 *tmpvec3 = (vec3 *)malloc( sizeof( vec3 ) );
                sscanf(&line[2], "%f %f %f", &tmpvec3->x, &tmpvec3->y, &tmpvec3->z);
                List_Push(vertList, tmpvec3);
            }

            if( line[0] == 'v' && line[1] == 't' ){
                coordCount++;
                vec2 *tmpvec2 = (vec2 *)malloc( sizeof( vec2 ) );
                sscanf(&line[3], "%f %f", &tmpvec2->x, &tmpvec2->y);
                List_Push(coordList, tmpvec2);
            }

            if( line[0] == 'v' && line[1] == 'n' ){
                normCount++;
                vec3 *tmpvec3 = (vec3 *)malloc( sizeof( vec3 ) );
                sscanf(&line[3], "%f %f %f", &tmpvec3->x, &tmpvec3->y, &tmpvec3->z);
                List_Push(normList, tmpvec3);
            }

            if( line[0] == 'f' && line[1] == ' ' ){
                faceCount++;
                obj_face_t *tmpface = (obj_face_t *)malloc( sizeof( obj_face_t ) );
                sscanf(&line[2], "%i/%i/%i %i/%i/%i %i/%i/%i", &fa[0], &fa[1], &fa[2], &fa[3], &fa[4], &fa[5], &fa[6], &fa[7], &fa[8]);
                //*tmpface = Face_Create( 3, vec3i_arrayToVec3ip(9, fa) );
                tmpface->vcount = 3;
                tmpface->indexes = vec3i_arrayToVec3ip(9, fa);
                List_Push(faceList, tmpface);
            }
        }
    }

    fclose(fp);

    vec3 *tmpv3 = NULL;
    vec2 *tmpv2 = NULL;
    obj_face_t *tmpf = NULL;

    model->vertCount = vertCount;
    model->vertices = (vec3 *)malloc(sizeof(vec3) * vertCount);
    tmpnode = List_Head(vertList);
    tmpv3 = (vec3 *)List_Data(tmpnode);
    for(i = 0; i < vertCount; i++){
        tmpv3 = List_Data(tmpnode);
        model->vertices[i] = *tmpv3;
        tmpnode = List_Next(tmpnode);
    } tmpv3 = NULL;

    model->coordCount = coordCount;
    model->textcoords = (vec2 *)malloc(sizeof(vec2) * coordCount);
    tmpnode = List_Head(coordList);
    tmpv2 = (vec2 *)List_Data(tmpnode);
    for(i = 0; i < coordCount; i++){
        tmpv2 = List_Data(tmpnode);
        model->textcoords[i] = *tmpv2;
        tmpnode = List_Next(tmpnode);
    } tmpv2 = NULL;

    model->normCount = normCount;
    model->normals = (vec3 *)malloc(sizeof(vec3) * normCount);
    tmpnode = List_Head(normList);
    tmpv3 = (vec3 *)List_Data(tmpnode);
    for(i = 0; i < normCount; i++){
        tmpv3 = List_Data(tmpnode);
        model->normals[i] = *tmpv3;
        tmpnode = List_Next(tmpnode);
    }

    model->trisCount = faceCount;
    model->faces = (obj_face_t *)malloc(sizeof(obj_face_t) * faceCount);
    tmpnode = List_Head(faceList);
    tmpf = (obj_face_t *)List_Data(tmpnode);
    for(i = 0; i < faceCount; i++){
        tmpf = List_Data(tmpnode);
        model->faces[i] = *tmpf;
        tmpnode = List_Next(tmpnode);
    }

    model->diffmap = (texture_t *)malloc(sizeof(texture_t));
    model->diffmap->bitmap = Bitmap_LoadPPM6("/home/pavel/Workspace/Project/C-Software-Renderer/assets/texture.ppm");

    List_Destroy(vertList);
    List_Destroy(coordList);
    List_Destroy(normList);
    List_Destroy(faceList);

    return model;
}

void Model_SetPosition ( obj_model_t *model, vec3 pos ){
    model->position = pos;
}

void Model_SetRotation ( obj_model_t *model, vec3 rot ){
    model->rotation = rot;
}

void Model_SetScale ( obj_model_t *model, vec3 scale ){
    model->scale = scale;
}

void Model_SetBaseColor ( obj_model_t *model, color_t color ){
    model->baseColor = color;
}

void Model_Test(obj_model_t *model){
    int i;
    if(model == NULL){
        printf("====================\n");
        printf("Warning: Bad model test!\n");
        return;
    }
    printf("====================\n");
    printf("Model Test(obj parser):\n\tModel name: \"%s\"\n\tVertex count: %d, Face count: %d, Triangle count: %d\n",
           model->name, model->vertCount, model->trisCount / 2, model->trisCount);

    printf("\tPosition: ");
    vec3_print(model->position);
    printf("\tRotation: ");
    vec3_print(model->rotation);
    printf("\tScale: ");
    vec3_print(model->scale);

    printf("Vertices: \n");
    for(i = 0; i < model->vertCount; i++){
        vec3_print(model->vertices[i]);
    }

    printf("Normals: \n");
    for(i = 0; i < model->trisCount * 2; i++){
        vec3_print(model->normals[i]);
    }

    printf("Texture Coordinates: \n");
    for(i = 0; i < model->trisCount * 2; i++){
        vec2_print(model->textcoords[i]);
    }

    printf("Faces: \n");
    for(i = 0; i < model->trisCount; i++){
        Face_Print(model->faces[i]);
    }
    printf("====================\n");
}

void Model_Rename(obj_model_t *model, char *new_name){
    strcpy (model->name, new_name);
}

obj_model_t *Model_FindByName( char *name ){
    obj_model_t *mod = NULL;
    return mod;
}

vec3 Model_GetPosition( obj_model_t *model ){
    return model->position;
}

vec3 Model_GetRotation( obj_model_t *model ){
    return model->rotation;
}

vec3 Model_GetScale( obj_model_t *model ){
    return model->scale;
}

color_t Model_GetBaseColor( obj_model_t *model ){
    return model->baseColor;
}

int Model_GetVertCount( obj_model_t *model ){
    return model->vertCount;
}

int Model_GetTrisCount( obj_model_t *model ){
    return model->trisCount;
}

texture_t *Model_GetTexture( obj_model_t *model ){
    return model->diffmap;
}

void Model_Free(obj_model_t *model){
    if(model == NULL){
        printf("Warning: Free request for NULL model pointer!");
        return;
    }

    if( !model->vertices )
        free(model->vertices);
    if( !model->normals )
        free(model->normals);
    if( !model->textcoords )
        free(model->textcoords);
    if( !model->faces ){
        int i;
        for(i = 0; i < model->trisCount; i++)
            free(model->faces[i].indexes);
        free(model->faces);
    }

    if( !model->diffmap ){
        Bitmap_Free(model->diffmap->bitmap);
        free(model->diffmap);
    }
    //free(specmap);
    //free(normalmap);
    //free(lightmap);

    model->vertices   = NULL;
    model->normals    = NULL;
    model->textcoords = NULL;
    model->faces      = NULL;
    model->diffmap    = NULL;

    free(model);
    model = NULL;
}
