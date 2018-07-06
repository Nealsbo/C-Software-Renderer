#ifndef LIBMATH_H_INCLUDED
#define LIBMATH_H_INCLUDED

//###############
//#   Typedef   #
//###############

typedef struct { float x, y; }              vec2;
typedef struct { float x, y, z; }           vec3;
typedef struct { float x, y, z, w; }        vec4;

typedef struct { int x, y; }                vec2i;
typedef struct { int x, y, z; }             vec3i;

typedef struct { float m[9]; }              mat3;
typedef struct { float m[16]; }             mat4;

typedef struct { vec3 origin, direction; }  ray;

typedef struct { vec3 pos, color;
				 ray  origin, normal; }	    ray_hit;

//#####################
//#   Function Init   #
//#####################

//###############
//#   Vector2   #
//###############

//Int
vec2i vec2i_create (int x, int y);
vec2i vec2i_createv(vec2i v);

vec2i vec2i_add(vec2i v1, vec2i v2);
vec2i vec2i_sub(vec2i v1, vec2i v2);
vec2i vec2i_mlt(vec2i v1,  float f);
vec2i vec2i_neg(vec2i v1);

void  vec2i_print (vec2i v);
void  vec2i_printp(void *vp);


//Float
vec2  vec2_create (float x, float y);
vec2  vec2_createv(vec2 v);

vec2  vec2_add(vec2 v1, vec2 v2);
vec2  vec2_sub(vec2 v1, vec2 v2);
vec2  vec2_mlt(vec2 v1, float f);
float vec2_dot(vec2 v1, vec2 v2);
vec2  vec2_neg(vec2 v1);
vec2  vec2_nrm(vec2 v1);
float vec2_lng(vec2 v1);

vec2  vec2_lerp(vec2 v1, vec2 v2, float value);

void  vec2_print (vec2 v);
void  vec2_printp(void *vp);


//###############
//#   Vector3   #
//###############

//int

vec3i vec3i_create (int x, int y, int z);
vec3i vec3i_createv(vec3i v);
vec3i *vec3i_arrayToVec3ip(int size, int *ar);

vec3i vec3i_add(vec3i v1, vec3i v2);
vec3i vec3i_sub(vec3i v1, vec3i v2);
vec3i vec3i_mlt(vec3i v1,  float f);
vec3i vec3i_neg(vec3i v1);

void  vec3i_print (vec3i v);
void  vec3i_printp(void *vp);

//Float

vec3  vec3_create (float x, float y, float z);
vec3  vec3_createv(vec3 v);

vec3  vec3_add(vec3 v1, vec3 v2);
vec3  vec3_sub(vec3 v1, vec3 v2);
vec3  vec3_mlt(vec3 v1, float f);
float vec3_dot(vec3 v1, vec3 v2);
vec3  vec3_crs(vec3 v1, vec3 v2);
vec3  vec3_neg(vec3 v1);
vec3  vec3_nrm(vec3 v1);
float vec3_lng(vec3 v1);

vec3  vec3_lerp   (vec3 v1, vec3 v2, float value);
vec2i vec3_toVec2i(vec3 v);
vec2  vec3_toVec2 (vec3 v);
vec4  vec3_toVec4 (vec3 v);
vec3  vec3_byMat4 (vec3 v, mat4 m);

void  vec3_print (vec3 v);
void  vec3_printp(void *vp);

//###############
//#   Vector4   #
//###############

vec4  vec4_create (float x, float y, float z, float w);
vec4  vec4_createv(vec4 v);

vec4  vec4_add(vec4 v1, vec4 v2);
vec4  vec4_sub(vec4 v1, vec4 v2);
vec4  vec4_mlt(vec4 v1, float f);
float vec4_dot(vec4 v1, vec4 v2);
vec4  vec4_crs(vec4 v1, vec4 v2);
vec4  vec4_neg(vec4 v1);
vec4  vec4_nrm(vec4 v1);
float vec4_lng(vec4 v1);
float vec4_max(vec4 v1);

vec4  vec4_lerp   (vec4 v1, vec4 v2, float value);
vec4  vec4_pdiv   (vec4 v);
vec2i vec4_toVec2i(vec4 v);
vec2  vec4_toVec2 (vec4 v);
vec3  vec4_toVec3 (vec4 v, int byw);
vec4  vec4_byMat4 (vec4 v, mat4 m);

void  vec4_print (vec4 v);
void  vec4_printp(void *vp);

//###############
//#   Matrix4   #
//###############

mat4  mat4_create();

mat4  mat4_add(mat4 m1, mat4 m2);
mat4  mat4_sub(mat4 m1, mat4 m2);
mat4  mat4_mlt(mat4 m1, mat4 m2);
mat4  mat4_tsp(mat4 m1);
float mat4_det(mat4 m1);

mat4  mat4_translate(float x, float y, float z);
mat4  mat4_scale    (float x, float y, float z);
mat4  mat4_uscale   (float s);
mat4  mat4_rotationA(float x, float y, float z, float angle);
mat4  mat4_rotation (float x, float y, float z);
mat4  mat4_rotx     (float r);
mat4  mat4_roty     (float r);
mat4  mat4_rotz     (float r);

mat4  mat4_projection(float near, float far, float fov, float aspect);      //TO FIX
mat4  mat4_screen    (float halfW, float halfH);      //TO FIX
mat4  mat4_lookAt    (vec3 pos, vec3 front, vec3 up);

void  mat4_print (mat4 m1);
void  mat4_printp(void *mp);

//###########
//#   Ray   #
//###########

ray   ray_create    (vec3 o, vec3 d);
ray   ray_creater   (ray r1);
float ray_getLength (ray r);
void  ray_print     (ray r);

//######################
//#   Math Functions   #
//######################

float power_f_i(float x, int y);
float clamp    (float value, float lower, float higher);


#endif // LIBMATH_H_INCLUDED
