#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

typedef struct {
    vec3  position, front, up, right, worldUp;
    mat4  projection;
    float yaw, pitch;
    float fov, aspect;
    float near, far;
    float speed;
} camera_t;

camera_t *Camera_Init        ( vec3 pos, vec3 up, float yaw, float pitch, float fov, float aspect, float near, float far, float speed );

void     Camera_SetPosition  ( camera_t *cam, vec3 pos );
void     Camera_SetFront     ( camera_t *cam, vec3 dir );
void     Camera_SetFov       ( camera_t *cam, float fov );
void     Camera_SetAspect    ( camera_t *cam, float aspect );
void     Camera_SetProjMatrix( camera_t *cam, float fov, float znear, float zfar, float aspect );       //TO FIX
void     Camera_UpdateMatrix ( camera_t *cam );
void     Camera_SetNear      ( camera_t *cam, float n);
void     Camera_SetFar       ( camera_t *cam, float f);
void     Camera_SetSpeed     ( camera_t *cam, float s);

vec3     Camera_GetPosition  ( camera_t *cam );
vec3     Camera_GetFront     ( camera_t *cam );
vec3     Camera_GetRight     ( camera_t *cam );
float    Camera_GetFov       ( camera_t *cam );
float    Camera_GetAspect    ( camera_t *cam );
void     Camera_GetMatrix    ( camera_t *cam, mat4 cam_mat );
void     Camera_GetViewProj  ( camera_t *cam, mat4 cam_mat );
float    Camera_GetNear      ( camera_t *cam );
float    Camera_GetFar       ( camera_t *cam );
float    Camera_GetSpeed     ( camera_t *cam );

void     Camera_PrintInfo    ( camera_t *cam );

void     Camera_ProcMovement ( camera_t *cam, int m_type, float delta );
void     Camera_AddPosition  ( camera_t *cam, vec3 step );      //TODO
void     Camera_RotateByVec  ( camera_t *cam, vec3 rot );       //TODO

static void Camera_UpdateVectors( camera_t *cam );

#endif // CAMERA_H_INCLUDED
