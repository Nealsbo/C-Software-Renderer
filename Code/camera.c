#include "srdefs.h"
#include "camera.h"

camera_t *Camera_Init( vec3 pos, vec3 dir, vec3 up, float fov, float aspect, float near, float far ){
    camera_t *cam   = (camera_t *)malloc(sizeof(camera_t));
    cam->position   = pos;
    cam->direction  = dir;
    cam->up         = up;
    cam->right      = vec3_crs( cam->direction, cam->up );
    cam->fov        = fov;
    cam->aspect     = aspect;
    cam->near       = near;
    cam->far        = far;
    cam->projection = mat4_projection( cam->near, cam->far, cam->fov, cam->aspect );
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

mat4 Camera_GetViewProj ( camera_t *cam ){
    return mat4_init();
}

void Camera_PrintInfo ( camera_t *cam ){
    printf("Position: ");
    vec3_print(cam->position);
    printf("Direction: ");
    vec3_print(cam->direction);
    printf("Up: ");
    vec3_print(cam->up);
    printf("Right: ");
    vec3_print(cam->right);
    printf("Fov: %f\n", cam->fov);
    printf("Projection Matrix:\n");
    mat4_print(cam->projection);
}

//##########################
//###   Camera control   ###
//##########################

void Camera_PerfMovement ( camera_t *cam, int m_type, float value ){
    vec3 curr_dir = Camera_GetDirection(cam);
    vec3 move, temp;
    switch( m_type ){
    case 1:
        move = vec3_mlt( vec3_nrm( curr_dir ), value );
        break;

    case 2:
        move = vec3_mlt( vec3_nrm( curr_dir ), -value );
        break;

    case 3:
        temp = vec3_mlt( vec3_nrm( curr_dir ), value );
        move = vec3_create( temp.z, temp.y, -temp.x );
        break;

    case 4:
        temp = vec3_mlt( vec3_nrm( curr_dir ), value );
        move = vec3_create( -temp.z, temp.y, temp.x );
        break;
    }
    Camera_MoveByVec( cam, move );
}

void Camera_MoveByVec ( camera_t *cam, vec3 step ){
    Camera_SetPosition( cam, vec3_add( Camera_GetPosition( cam ), step ) );
}
