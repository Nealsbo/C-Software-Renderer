#include "srdefs.h"
#include "camera.h"

camera_t *Camera_Init( vec3 pos, vec3 dir, vec3 up, float fov, float aspect, float near, float far ){
    camera_t *cam   = (camera_t *)malloc(sizeof(camera_t));
    cam->position   = pos;
    cam->direction  = dir;
    cam->up         = up;
    cam->fov        = fov;
    cam->aspect     = aspect;
    cam->near       = near;
    cam->far        = far;
    Camera_UpdateMatrix(cam);
    return cam;
}

//###########################
//###   Set camera data   ###
//###########################

void Camera_SetPosition( camera_t *cam, vec3 pos ){
    cam->position = pos;
}

void Camera_SetDirection( camera_t *cam, vec3 dir ){
    cam->direction = dir;
}

void Camera_SetFov( camera_t *cam, float fov ){
    cam->fov = fov;
}

void Camera_SetAspect( camera_t *cam, float aspect ){
    cam->aspect = aspect;
}

void Camera_SetProjMatrix( camera_t *cam, float fov, float znear, float zfar, float aspect ){
    cam->fov = fov;
    cam->projection = mat4_projection( znear, zfar, fov, aspect );
}

void Camera_UpdateMatrix ( camera_t *cam ){
    cam->projection = mat4_projection( cam->near, cam->far, cam->fov, cam->aspect );
}

void Camera_SetNear ( camera_t *cam, float n){
    cam->near = n;
}

void Camera_SetFar ( camera_t *cam, float f){
    cam->far = f;
}

//###########################
//###   Get camera data   ###
//###########################

vec3 Camera_GetPosition( camera_t *cam ){
    return cam->position;
}

vec3 Camera_GetDirection( camera_t *cam ){
    return cam->direction;
}

float Camera_GetFov( camera_t *cam ){
    return cam->fov;
}

float Camera_GetAspect( camera_t *cam ){
    return cam->aspect;
}

float Camera_GetNear ( camera_t *cam ){
    return cam->near;
}
float Camera_GetFar ( camera_t *cam ){
    return cam->far;
}

void Camera_PrintInfo( camera_t *cam ){
    printf("Position: ");
    vec3_print(cam->position);
    printf("Direction: ");
    vec3_print(cam->direction);
    printf("Fov: %f\n", cam->fov);
    printf("Projection Matrix:\n");
    mat4_print(cam->projection);
}

//##########################
//###   Camera control   ###
//##########################
