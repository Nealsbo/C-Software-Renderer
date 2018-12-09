#include "srdefs.h"
#include "camera.h"

camera_t *Camera_Init( vec3 pos, vec3 up, float yaw, float pitch, float fov, float aspect, float near, float far, float speed ) {
    camera_t *cam   = ( camera_t * )malloc( sizeof(camera_t) );
    cam->position   = pos;
    cam->up         = up;
    cam->right      = vec3_crs( cam->front, cam->up );
    cam->worldUp    = up;
    cam->yaw        = yaw;
    cam->pitch      = pitch;
    cam->fov        = fov;
    cam->aspect     = aspect;
    cam->near       = near;
    cam->far        = far;
    cam->speed      = speed;
    Camera_UpdateVectors( cam );
    cam->projection = mat4_projection( cam->near, cam->far, cam->fov, cam->aspect );
    return cam;
}

//###########################
//###   Set camera data   ###
//###########################

void Camera_SetPosition( camera_t *cam, vec3 pos ) {
    cam->position = pos;
}

void Camera_SetFront( camera_t *cam, vec3 front ) {
    cam->front = front;
}

void Camera_SetFov( camera_t *cam, float fov ) {
    cam->fov = fov;
}

void Camera_SetAspect( camera_t *cam, float aspect ) {
    cam->aspect = aspect;
}

void Camera_SetProjMatrix( camera_t *cam, float fov, float znear, float zfar, float aspect ) {
    cam->fov = fov;
    cam->projection = mat4_projection( znear, zfar, fov, aspect );
}

void Camera_UpdateMatrix ( camera_t *cam ) {
    cam->projection = mat4_projection( cam->near, cam->far, cam->fov, cam->aspect );
}

void Camera_SetNear ( camera_t *cam, float n) {
    cam->near = n;
}

void Camera_SetFar ( camera_t *cam, float f) {
    cam->far = f;
}

void Camera_SetSpeed ( camera_t *cam, float s) {
	cam->speed = s;
}

//###########################
//###   Get camera data   ###
//###########################

vec3 Camera_GetPosition( camera_t *cam ) {
    return cam->position;
}

vec3 Camera_GetFront( camera_t *cam ) {
    return cam->front;
}

vec3 Camera_GetRight( camera_t *cam ) {
    return cam->right;
}

float Camera_GetFov( camera_t *cam ) {
    return cam->fov;
}

float Camera_GetAspect( camera_t *cam ) {
    return cam->aspect;
}

float Camera_GetNear ( camera_t *cam ) {
    return cam->near;
}
float Camera_GetFar ( camera_t *cam ) {
    return cam->far;
}

mat4 Camera_GetViewProj ( camera_t *cam ) {
    return cam->projection;
}

float Camera_GetSpeed ( camera_t *cam ) {
    return cam->speed;
}

void Camera_PrintInfo ( camera_t *cam ) {
    printf("Position: "); vec3_print(cam->position);
    printf("Front: ");    vec3_print(cam->front);
    printf("Up: ");       vec3_print(cam->up);
    printf("Right: ");    vec3_print(cam->right);
    
    printf("Fov: %f; Aspect: %f\n", cam->fov, cam->aspect);
    printf("Projection Matrix:\n"); mat4_print(cam->projection);
}

//##########################
//###   Camera control   ###
//##########################

void Camera_ProcMovement ( camera_t *cam, int m_type/*, float delta*/ ) {
    vec3  move, front, right;
    float speed_value = Camera_GetSpeed( cam ) /* * delta*/;
    switch( m_type ){
    case 1:
        move = vec3_mlt( Camera_GetFront( cam ), speed_value );
        break;

    case 2:
        move = vec3_mlt( Camera_GetFront( cam ), -speed_value );
        break;

    case 3:
        move = vec3_mlt( Camera_GetRight( cam ), speed_value );
        break;

    case 4:
        move = vec3_mlt( Camera_GetRight( cam ), -speed_value );
        break;
    }
    Camera_AddPosition( cam, move );
}

void Camera_AddPosition ( camera_t *cam, vec3 step ) {
    Camera_SetPosition( cam, vec3_add( Camera_GetPosition( cam ), step ) );
}

static void Camera_UpdateVectors( camera_t *cam ) {
	vec3 front = vec3_create( cosf( to_radians(cam->yaw) ) * cosf( to_radians(cam->pitch)),
	                          sinf( to_radians(cam->pitch) ),
	                          sinf( to_radians(cam->yaw) ) * cosf( to_radians(cam->pitch)));
	front      = vec3_nrm(front);
	cam->front = front;
	cam->right = vec3_nrm( vec3_crs(cam->front, cam->worldUp) );
	cam->up    = vec3_nrm( vec3_crs(cam->right, cam->front)   );
}
