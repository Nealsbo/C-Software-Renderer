#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "srdefs.h"
#include "libmath.h"

static mat4 zero_mat = {
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f };

static mat4 identity_mat = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f };

//###############
//#   Vector2   #
//###############

//Int

vec2i vec2i_create(int x, int y){
    vec2i v = {x, y};
    return v;
}

vec2i vec2i_createv(vec2i v){
    vec2i v1 = {v.x, v.y};
    return v1;
}

vec2i vec2i_add(vec2i v1, vec2i v2){
    vec2i v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    return v3;
}
vec2i vec2i_sub(vec2i v1, vec2i v2){
    vec2i v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    return v3;
}

vec2i vec2i_mlt(vec2i v1,  float f){
    vec2i v3;
    v3.x = v1.x * f;
    v3.y = v1.y * f;
    return v3;
}

vec2i vec2i_neg(vec2i v1){
    vec2i v2;
    v2.x = -v1.x;
    v2.y = -v1.y;
    return v2;
}

vec2  vec2i_toVec2(vec2i v1){
    vec2 v2 = {(float)v1.x, (float)v1.y};
    return v2;
}

void vec2i_print(vec2i v){
    printf("Vector2i:{ %d, %d )\n", v.x, v.y);
}

void vec2i_printp(void *vp){
    vec2i *v = vp;
    printf("Vec2i: %d %d\n", v->x, v->y);
}

//Float

vec2 vec2_create(float x, float y){
    vec2 v = {x, y};
    return v;
}

vec2 vec2_createv(vec2 v){
    vec2 v1 = {v.x, v.y};
    return v1;
}

vec2 vec2_add(vec2 v1, vec2 v2){
    vec2 v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    return v3;
}

vec2 vec2_sub(vec2 v1, vec2 v2){
    vec2 v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    return v3;
}

vec2 vec2_mlt(vec2 v1, float f){
    vec2 v3;
    v3.x = v1.x * f;
    v3.y = v1.y * f;
    return v3;
}

float vec2_dot(vec2 v1, vec2 v2){
    float res = v1.x * v2.x + v1.y * v2.y;
    return res;
}

vec2 vec2_neg(vec2 v1){
    vec2 v2;
    v2.x = -v1.x;
    v2.y = -v1.y;
    return v2;
}

vec2 vec2_nrm(vec2 v1){
    vec2 v2;
    float len = vec2_lng(v1);
    v2.x = v1.x / len;
    v2.y = v1.y / len;
    return v2;
}

float vec2_lng(vec2 v1){
    float len = sqrtf(v1.x * v1.x + v1.y * v1.y);
    return len;
}

vec2i vec2_toVec2i(vec2 v){
    vec2i v2 = {(int)v.x, (int)v.y};
    return v2;
}

vec3 vec2_toVec3(vec2 v){
    vec3 v2 = {v.x, v.y, 1.0};
    return v2;
}

vec2 vec2_lerp (vec2 v1, vec2 v2, float value){
    return vec2_add( vec2_mlt( vec2_sub(v2, v1), value ), v1);
}

void vec2_print(vec2 v){
    printf("Vector2 :{ %f, %f )\n", v.x, v.y);
}

void vec2_printp (void *vp){
    vec2 *v = vp;
    printf("Vec2:  %f %f\n", v->x, v->y);
}

//###############
//#   Vector3   #
//###############

//Int

vec3i vec3i_create(int x, int y, int z){
    vec3i v = {x, y, z};
    return v;
}

vec3i  vec3i_createv(vec3i v){
    vec3i v1 = {v.x, v.y, v.z};
    return v1;
}

vec3i *vec3i_arrayToVec3ip(int size, int *ar){
    int i;
    vec3i *v = malloc(sizeof(vec3i) * (size / 3));
    for(i = 0; i < size / 3; i++)
        v[i] = (vec3i){ar[i*3], ar[i*3+1], ar[i*3+2]};
    return v;
}

vec3i vec3i_add(vec3i v1, vec3i v2){
    vec3i v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    v3.z = v1.z + v2.z;
    return v3;
}

vec3i vec3i_sub(vec3i v1, vec3i v2){
    vec3i v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    v3.z = v1.z - v2.z;
    return v3;
}

vec3i vec3i_mlt(vec3i v1,  float f){
    vec3i v3;
    v3.x = v1.x * f;
    v3.y = v1.y * f;
    v3.z = v1.z * f;
    return v3;
}

vec3i vec3i_neg(vec3i v1){
    vec3i v2;
    v2.x = -v1.x;
    v2.y = -v1.y;
    v2.z = -v1.z;
    return v2;
}

void vec3i_print(vec3i v){
    printf("Vector3i:{ %d, %d, %d )\n", v.x, v.y, v.z);
}

void vec3i_printp(void *vp){
    vec3i *v = vp;
    printf("Vec3i: %d %d %d\n", v->x, v->y, v->z);
}

//Float

vec3 vec3_create(float x, float y, float z){
    vec3 v = {x, y, z};
    return v;
}

vec3 vec3_createv(vec3 v){
    vec3 v1 = {v.x, v.y, v.z};
    return v1;
}

vec3 vec3_add(vec3 v1, vec3 v2){
    vec3 v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    v3.z = v1.z + v2.z;
    return v3;
}

vec3 vec3_sub(vec3 v1, vec3 v2){
    vec3 v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    v3.z = v1.z - v2.z;
    return v3;
}

vec3 vec3_mlt(vec3 v1, float f){
    vec3 v3;
    v3.x = v1.x * f;
    v3.y = v1.y * f;
    v3.z = v1.z * f;
    return v3;
}

vec3 vec3_mltv(vec3 v1, vec3 v2){
    vec3 v3;
    v3.x = v1.x * v2.x;
    v3.y = v1.y * v2.y;
    v3.z = v1.z * v2.z;
    return v3;
}

float vec3_dot(vec3 v1, vec3 v2){
    float res = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return res;
}

vec3 vec3_crs(vec3 v1, vec3 v2){
    vec3 v3;
    v3.x = v1.y * v2.z - v1.z * v2.y;
    v3.y = v1.z * v2.x - v1.x * v2.z;
    v3.z = v1.x * v2.y - v1.y * v2.x;
    return v3;
}

vec3 vec3_neg(vec3 v1){
    vec3 v2;
    v2.x = -v1.x;
    v2.y = -v1.y;
    v2.z = -v1.z;
    return v2;
}

vec3 vec3_nrm(vec3 v1){
    vec3 v2;
    float len = vec3_lng(v1);
    v2.x = v1.x / len;
    v2.y = v1.y / len;
    v2.z = v1.z / len;
    return v2;
}

float vec3_lng(vec3 v1){
    float len = sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
    return len;
}

vec3 vec3_lerp (vec3 v1, vec3 v2, float value){
    return vec3_add( vec3_mlt( vec3_sub(v2, v1), value ), v1);
}

vec3 vec3_refl(vec3 v1, vec3 v2) {
	return vec3_sub( v1, vec3_mlt( vec3_mlt( v2, vec3_dot( v1, v2 ) ), 2.0f ) );
}

vec3 vec3_pdiv (vec3 v){
    return vec3_create(v.x / v.z, v.y / v.z, v.z);
}

vec2 vec3_toVec2(vec3 v){
    vec2 v2 = {v.x, v.y};
    return v2;
}

vec2i vec3_toVec2i(vec3 v){
    vec2i v2 = {(int)v.x, (int)v.y};
    return v2;
}

vec4 vec3_toVec4(vec3 v){
    vec4 v2 = {v.x, v.y, v.z, 1};
    return v2;
}

vec3 vec3_byMat4(vec3 v, mat4 m){
    vec4 v2 = {m[ 0] * v.x + m[ 1] * v.y + m[ 2] * v.z + m[ 3],
               m[ 4] * v.x + m[ 5] * v.y + m[ 6] * v.z + m[ 7],
               m[ 8] * v.x + m[ 9] * v.y + m[10] * v.z + m[11],
               m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15],};
    /*
    vec3 v2 = {m[ 0] * v.x + m[ 1] * v.y + m[ 2] * v.z,
               m[ 4] * v.x + m[ 5] * v.y + m[ 6] * v.z,
               m[ 8] * v.x + m[ 9] * v.y + m[10] * v.z};
    */
    return vec4_toVec3( vec4_pdiv( v2 ) );
}

void vec3_print(vec3 v){
    printf("Vector3:{ %f, %f, %f )\n", v.x, v.y, v.z);
}

void vec3_printp (void *vp){
    vec3 *v = vp;
    printf("Vec3:  %f %f %f\n", v->x, v->y, v->z);
}

//###############
//#   Vector4   #
//###############

vec4 vec4_create(float x, float y, float z, float w){
    vec4 v = {x, y, z, w};
    return v;
}

vec4 vec4_createv(vec4 v){
    vec4 v1 = {v.x, v.y, v.z, v.w};
    return v1;
}

vec4 vec4_add(vec4 v1, vec4 v2){
    vec4 v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    v3.z = v1.z + v2.z;
    v3.w = v1.w + v2.w;
    return v3;
}

vec4 vec4_sub(vec4 v1, vec4 v2){
    vec4 v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    v3.z = v1.z - v2.z;
    v3.w = v1.w - v2.w;
    return v3;
}

vec4 vec4_mlt(vec4 v1, float f){
    vec4 v3;
    v3.x = v1.x * f;
    v3.y = v1.y * f;
    v3.z = v1.z * f;
    v3.w = v1.w * f;
    return v3;
}

float vec4_dot(vec4 v1, vec4 v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

vec4 vec4_crs(vec4 v1, vec4 v2){
    vec4 v3;
    v3.x = v1.y * v2.z - v1.z * v2.y;
    v3.y = v1.z * v2.x - v1.x * v2.z;
    v3.z = v1.x * v2.y - v1.y * v2.x;
    v3.w = 1;
    return v3;
}

vec4 vec4_neg(vec4 v1){
    return vec4_create( -v1.x, -v1.y, -v1.z, -v1.w );
}

vec4 vec4_nrm(vec4 v1){
    float length = sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z + v1.w * v1.w);
    return vec4_create( v1.x / length, v1.y / length, v1.z / length, v1.w / length );
}

float vec4_lng(vec4 v1){
    return sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z + v1.w * v1.w);
}

float vec4_max(vec4 v1){
    return MAX( MAX( v1.x, v1.y ), MAX( v1.z, v1.w ) );
}

vec4 vec4_lerp (vec4 v1, vec4 v2, float value){
    return vec4_add( vec4_mlt( vec4_sub(v2, v1), value ), v1);
}

vec4 vec4_pdiv (vec4 v){
    return vec4_create(v.x / v.w, v.y / v.w, v.z / v.w, v.w);
}

vec2i vec4_toVec2i(vec4 v){
    return vec2i_create((int)v.x, (int)v.y);
}

vec2 vec4_toVec2 (vec4 v){
    return vec2_create(v.x, v.y);
}

vec3 vec4_toVec3(vec4 v){
    vec3 v2 = {v.x, v.y, v.z};
    return v2;
}

/*
vec3 vec4_toVec3(vec4 v, int byw){
    if(byw){
        vec3 v2 = {v.x/v.w, v.y/v.w, v.z/v.w};
        return v2;
    } else {
        vec3 v2 = {v.x, v.y, v.z};
        return v2;
    }
}
*/

vec4 vec4_byMat4(vec4 v, mat4 m){
    vec4 v2 = {m[ 0] * v.x + m[ 1] * v.y + m[ 2] * v.z + m[ 3] * v.w,
               m[ 4] * v.x + m[ 5] * v.y + m[ 6] * v.z + m[ 7] * v.w,
               m[ 8] * v.x + m[ 9] * v.y + m[10] * v.z + m[11] * v.w,
               m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w};
    return v2;
}

void vec4_print(vec4 v){
    printf("Vector4:{ %f, %f, %f, %f )\n", v.x, v.y, v.z, v.w);
}

void vec4_printp (void *vp){
    vec4 *v = vp;
    printf("Vec4:  %f %f %f %f\n", v->x, v->y, v->z, v->w);
}

//###############
//#   Matrix4   #
//###############

void mat4_add( mat4 m1, mat4 m2, mat4 dest ){
    int i;
    for(i = 0; i < 16; i++){
        dest[i] = m1[i] + m2[i];
    }
}

void mat4_sub( mat4 m1, mat4 m2, mat4 dest ){
    int i;
    for(i = 0; i < 16; i++){
        dest[i] = m1[i] - m2[i];
    }
}

void mat4_mlt( mat4 m1, mat4 m2, mat4 dest ){
    int i, j, k;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            for(k = 0; k < 4; k++)
                dest[i * 4 + j] += m1[i * 4 + k] * m2[4 * k + j];
}

void mat4_tsp( mat4 m1, mat4 dest ){
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            dest[i * 4 + j] = m1[i + j * 4];
}

void mat4_inv( mat4 m1, mat4 dest ){
    mat4 m;
    float det;
    int i, j;

    dest[0]  =  m1[5] * m1[10] * m1[15] - m1[5] * m1[11] * m1[14] - m1[9]  * m1[6]  * m1[15] + m1[9]  * m1[7]  * m1[14] + m1[13] * m1[6]  * m1[11] - m1[13] * m1[7]  * m1[10];
    dest[4]  = -m1[4] * m1[10] * m1[15] + m1[4] * m1[11] * m1[14] + m1[8]  * m1[6]  * m1[15] - m1[8]  * m1[7]  * m1[14] - m1[12] * m1[6]  * m1[11] + m1[12] * m1[7]  * m1[10];
    dest[8]  =  m1[4] * m1[9]  * m1[15] - m1[4]  * m1[11] * m1[13] - m1[8]  * m1[5] * m1[15] + m1[8]  * m1[7] * m1[13] + m1[12] * m1[5] * m1[11] - m1[12] * m1[7] * m1[9];
    dest[12] = -m1[4] * m1[9]  * m1[14] + m1[4]  * m1[10] * m1[13] + m1[8]  * m1[5] * m1[14] - m1[8]  * m1[6] * m1[13] - m1[12] * m1[5] * m1[10] + m1[12] * m1[6] * m1[9];
    dest[1]  = -m1[1] * m1[10] * m1[15] + m1[1]  * m1[11] * m1[14] + m1[9]  * m1[2] * m1[15] - m1[9]  * m1[3] * m1[14] - m1[13] * m1[2] * m1[11] + m1[13] * m1[3] * m1[10];
    dest[5]  =  m1[0] * m1[10] * m1[15] - m1[0]  * m1[11] * m1[14] - m1[8]  * m1[2] * m1[15] + m1[8]  * m1[3] * m1[14] + m1[12] * m1[2] * m1[11] - m1[12] * m1[3] * m1[10];
    dest[9]  = -m1[0] * m1[9]  * m1[15] + m1[0]  * m1[11] * m1[13] + m1[8]  * m1[1] * m1[15] - m1[8]  * m1[3] * m1[13] - m1[12] * m1[1] * m1[11] + m1[12] * m1[3] * m1[9];
    dest[13] =  m1[0] * m1[9]  * m1[14] - m1[0]  * m1[10] * m1[13] - m1[8]  * m1[1] * m1[14] + m1[8]  * m1[2] * m1[13] + m1[12] * m1[1] * m1[10] - m1[12] * m1[2] * m1[9];
    dest[2]  =  m1[1] * m1[6]  * m1[15] - m1[1]  * m1[7] * m1[14] - m1[5]  * m1[2] * m1[15] + m1[5]  * m1[3] * m1[14] + m1[13] * m1[2] * m1[7] - m1[13] * m1[3] * m1[6];
    dest[6]  = -m1[0] * m1[6]  * m1[15] + m1[0]  * m1[7] * m1[14] + m1[4]  * m1[2] * m1[15] - m1[4]  * m1[3] * m1[14] - m1[12] * m1[2] * m1[7] + m1[12] * m1[3] * m1[6];
    dest[10] =  m1[0] * m1[5]  * m1[15] - m1[0]  * m1[7] * m1[13] - m1[4]  * m1[1] * m1[15] + m1[4]  * m1[3] * m1[13] + m1[12] * m1[1] * m1[7] - m1[12] * m1[3] * m1[5];
    dest[14] = -m1[0] * m1[5]  * m1[14] + m1[0]  * m1[6] * m1[13] + m1[4]  * m1[1] * m1[14] - m1[4]  * m1[2] * m1[13] - m1[12] * m1[1] * m1[6] + m1[12] * m1[2] * m1[5];
    dest[3]  = -m1[1] * m1[6]  * m1[11] + m1[1] * m1[7] * m1[10] + m1[5] * m1[2] * m1[11] - m1[5] * m1[3] * m1[10] - m1[9] * m1[2] * m1[7] + m1[9] * m1[3] * m1[6];
    dest[7]  =  m1[0] * m1[6]  * m1[11] - m1[0] * m1[7] * m1[10] - m1[4] * m1[2] * m1[11] + m1[4] * m1[3] * m1[10] + m1[8] * m1[2] * m1[7] - m1[8] * m1[3] * m1[6];
    dest[11] = -m1[0] * m1[5]  * m1[11] + m1[0] * m1[7] * m1[9] + m1[4] * m1[1] * m1[11] - m1[4] * m1[3] * m1[9] - m1[8] * m1[1] * m1[7] + m1[8] * m1[3] * m1[5];
    dest[15] =  m1[0] * m1[5]  * m1[10] - m1[0] * m1[6] * m1[9] - m1[4] * m1[1] * m1[10] + m1[4] * m1[2] * m1[9] + m1[8] * m1[1] * m1[6] - m1[8] * m1[2] * m1[5];

    det = m1[0] * dest[0] + m1[1] * dest[4] + m1[2] * dest[8] + m1[3] * dest[12];

    det = 1.0 / det;

    for( i = 0; i < 16; i++ ) {
        dest[i] = dest[i] * det;
    }
}

void mat4_print( mat4 m1 ){
    int i;
    for( i = 0; i < 4; i++ )
        printf( "Matrix:{ %f, %f, %f, %f )\n", m1[4*i], m1[4*i + 1], m1[4*i + 2], m1[4*i + 3] );
    printf("\n");
}

void mat4_print_named( mat4 m1 , char *name ){
    int i;
    printf("Matrix name: %s\n", name);

    for( i = 0; i < 4; i++ )
        printf( "Matrix:{ %f, %f, %f, %f )\n", m1[4*i], m1[4*i + 1], m1[4*i + 2], m1[4*i + 3] );
    printf("\n");
}

void mat4_create( mat4 dest ){
    memcpy(dest, zero_mat, sizeof(mat4));
}

void mat4_identity( mat4 dest ){
    memcpy(dest, identity_mat, sizeof(mat4));
}

void mat4_setColVec2( mat4 m, vec2 v, int i ) {
	if( i >= 0 || i <= 3 ) {
        m[    i] = v.x;
        m[4 + i] = v.y;
	}
}

void mat4_setColVec3( mat4 m, vec3 v, int i ) {
	if( i >= 0 || i <= 3 ) {
        m[    i] = v.x;
        m[4 + i] = v.y;
        m[8 + i] = v.z;
	}
}

void mat4_setColVec4( mat4 m, vec4 v, int i ) {
	if( i >= 0 || i <= 3 ) {
        m[     i] = v.x;
        m[ 4 + i] = v.y;
        m[ 8 + i] = v.z;
        m[12 + i] = v.w;
	}
}

void mat4_setRowVec2( mat4 m, vec2 v, int i ) {
	if( i >= 0 || i <= 3 ) {
        m[i    ] = v.x;
        m[i + 1] = v.y;
	}
}

void mat4_setRowVec3( mat4 m, vec3 v, int i ) {
	if( i >= 0 || i <= 3 ) {
        m[i    ] = v.x;
        m[i + 1] = v.y;
        m[i + 2] = v.z;
	}
}

void mat4_setRowVec4( mat4 m, vec4 v, int i ) {
	if( i >= 0 || i <= 3 ) {
        m[i    ] = v.x;
        m[i + 1] = v.y;
        m[i + 2] = v.z;
        m[i + 3] = v.w;
	}
}

vec2  mat4_getColVec2( mat4 m, vec2 v, int i ) {
	return vec2_create( m[i], m[4 + i] );
}

vec3  mat4_getColVec3( mat4 m, vec3 v, int i ) {
	return vec3_create( m[i], m[4 + i], m[8 + i] );
}

vec4  mat4_getColVec4( mat4 m, vec4 v, int i ) {
	return vec4_create( m[i], m[4 + i], m[8 + i], m[12 + i] );
}

vec2  mat4_getRowVec2( mat4 m, vec2 v, int i ) {
	return vec2_create( m[i], m[i + 1]);
}

vec3  mat4_getRowVec3( mat4 m, vec3 v, int i ) {
	return vec3_create( m[i], m[i + 1], m[i + 2] );
}

vec4  mat4_getRowVec4( mat4 m, vec4 v, int i ) {
	return vec4_create( m[i], m[i + 1], m[i + 2], m[i + 3] );
}

void mat4_translate( mat4 dest, float x, float y, float z){
    mat4_copy( identity_mat, dest );
    dest[3] = x;
    dest[7] = y;
    dest[11] = z;
}

void mat4_scale( mat4 dest, float x, float y, float z ){
    mat4_copy( zero_mat, dest );
    dest[0] = x;
    dest[5] = y;
    dest[10] = z;
    dest[15] = 1.0f;
}

void mat4_uscale( mat4 dest, float s ){
    mat4_copy( zero_mat, dest );
    dest[0] = s;
    dest[5] = s;
    dest[10] = s;
    dest[15] = 1.0f;
}

void mat4_rotationA( mat4 dest, float x, float y, float z, float angle ){
    float sin = sinf(angle);
    float cos = cosf(angle);

    mat4 m = {
              cos+x*x*(1-cos), x*y*(1-cos)-z*sin, x*z*(1-cos)+y*sin, 0,
            y*x*(1-cos)+z*sin,   cos+y*y*(1-cos), y*z*(1-cos)-x*sin, 0,
            z*x*(1-cos)-y*sin, z*y*(1-cos)+x*sin,   cos+z*z*(1-cos), 0,
                            0,                 0,                 0, 1 };
    mat4_copy(m, dest);
}

void mat4_rotation( mat4 dest, float x, float y, float z ){
    mat4 mx, my, mz, tmp;
    mat4_create(tmp);
    mat4_create(dest);
    mat4_rotx(mx, x);
    mat4_roty(my, y);
    mat4_rotz(mz, z);
    mat4_mlt( my, mx, tmp );
    mat4_mlt( mz, tmp, dest );
    //mat4_print(mx);
    //mat4_print(my);
    //mat4_print(mz);
    //mat4_print(tmp);
    //mat4_print(dest);
}

void mat4_rotx( mat4 dest, float r ){
    mat4 m = {
        1.0f,    0.0f,   0.0f, 0.0f,
        0.0f,  cos(r), sin(r), 0.0f,
        0.0f, -sin(r), cos(r), 0.0f,
        0.0f,    0.0f,   0.0f, 1.0f };
    mat4_copy(m, dest);
}

void mat4_roty( mat4 dest, float r ){
    mat4 m = {
        cos(r), 0.0f, -sin(r), 0.0f,
          0.0f, 1.0f,    0.0f, 0.0f,
        sin(r), 0.0f,  cos(r), 0.0f,
          0.0f, 0.0f,    0.0f, 1.0f };
    mat4_copy(m, dest);
}

void mat4_rotz( mat4 dest, float r ){
    mat4 m = {
         cos(r), sin(r), 0.0f, 0.0f,
        -sin(r), cos(r), 0.0f, 0.0f,
           0.0f,   0.0f, 1.0f, 0.0f,
           0.0f,   0.0f, 0.0f, 1.0f };

    mat4_copy(m, dest);
}

void mat4_projection( mat4 dest, float n, float f, float fov, float aspect ){
    float s = 1.0f/tanf(fov * 0.5f * M_PI / 180.0f);
    float zr = n - f;
    
    mat4 m = {
            s/aspect, 0.0f,      0.0f,        0.0f,
                0.0f,    s,      0.0f,        0.0f,
                0.0f, 0.0f, (-n-f)/zr, 2.0f*n*f/zr,
                0.0f, 0.0f,     -1.0f,        0.0f };
    mat4_copy(m, dest);
}

void mat4_screen( mat4 dest, float halfW, float halfH ){
    mat4 m = {
            halfW,   0.0f,  0.0f, halfW,
             0.0f,  halfH,  0.0f, halfH,
             0.0f,   0.0f,  1.0f,  0.0f,
             0.0f,   0.0f,  0.0f,  1.0f };
    mat4_copy(m, dest);
}

void mat4_lookAt( mat4 dest, vec3 eye, vec3 center, vec3 up ){    
    vec3 f = vec3_nrm(vec3_sub(center, eye));
    vec3 r = vec3_crs(up, f);
    vec3 u = vec3_crs(f, r);

    mat4 m = {
             r.x,  u.x,  f.x, eye.x,
             r.y,  u.y,  f.y, eye.y,
             r.z,  u.z,  f.z, eye.z,
            0.0f, 0.0f, 0.0f,  1.0f };
    
    mat4_copy(m, dest);
}

void mat4_copy(mat4 src, mat4 dest){
    memcpy(dest, src, sizeof(mat4));
}

//###########
//#   Ray   #
//###########

ray ray_create(vec3 o, vec3 d){
    ray r = {o, d};
    return r;
}

ray ray_creater(ray r1){
    ray r = {r.origin, r.direction};
    return r;
}

vec3 ray_point(ray r, float t){
    vec3 p = vec3_add(r.origin, vec3_mlt(r.direction, t));
    return p;
}

ray ray_normalize (ray r){
    return ray_create(r.origin, vec3_nrm(r.direction));
}
/*
ray ray_reflect (ray r, ray n){
    
}
*/

float ray_getLength(ray r){
    return sqrtf( power_f_i((r.direction.x - r.origin.x), 2) +
                  power_f_i((r.direction.y - r.origin.y), 2) +
                  power_f_i((r.direction.z - r.origin.z), 2));
}

void ray_print(ray r){
    vec3_print(r.origin);
    vec3_print(r.direction);
}


//######################
//#   Math Functions   #
//######################

float to_radians( float degrees ) {
	return degrees * ( M_PI / 180.0f );
}

float to_degrees( float radians ) {
	return radians * ( 180.0f / M_PI );
}

float power_f_i( float x, int y ) {
    int i;
    float s = 1;
    for( i = y; i != 0; --i )
		s *= x;
    return s;
}

float clamp( float value, float lower, float higher ) {
    return (value > higher) ? higher : (value < lower) ? lower : value;
}
