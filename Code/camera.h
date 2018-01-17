#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

typedef struct {
    vec3 position, direction, up, right;
    mat4 projection;
    float fov, aspect;
    float near, far;
} camera_t;

camera_t *Camera_Init        ( vec3 pos, vec3 dir, vec3 up, float fov, float aspect, float near, float far );

void     Camera_SetPosition  ( camera_t *cam, vec3 pos );
void     Camera_SetDirection ( camera_t *cam, vec3 dir );
void     Camera_SetFov       ( camera_t *cam, float fov );
void     Camera_SetAspect    ( camera_t *cam, float aspect );
void     Camera_SetProjMatrix( camera_t *cam, float fov, float znear, float zfar, float aspect );       //TO FIX
void     Camera_UpdateMatrix ( camera_t *cam );
void     Camera_SetNear      ( camera_t *cam, float n);
void     Camera_SetFar       ( camera_t *cam, float f);

vec3     Camera_GetPosition  ( camera_t *cam );
vec3     Camera_GetDirection ( camera_t *cam );
float    Camera_GetFov       ( camera_t *cam );
float    Camera_GetAspect    ( camera_t *cam );
mat4     Camera_GetMatrix    ( camera_t *cam );
mat4     Camera_GetViewProj  ( camera_t *cam );
float    Camera_GetNear      ( camera_t *cam );
float    Camera_GetFar       ( camera_t *cam );

void     Camera_PrintInfo    ( camera_t *cam );

void     Camera_PerfMovement ( camera_t *cam, int m_type, float value );
void     Camera_MoveByVec    ( camera_t *cam, vec3 step );      //TODO
void     Camera_RotateByVec  ( camera_t *cam, vec3 rot );       //TODO

#endif // CAMERA_H_INCLUDED
