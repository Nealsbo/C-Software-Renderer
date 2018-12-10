#include "srdefs.h"

const int SCREEN_TICK_PER_FRAME = 1000 / FPS_LIMIT;

int quit = FALSE;

float cam_speed = 1.0f;

scene_t     *Scene              = NULL;
camera_t    *Cam                = NULL;

SDL_Window  *gWindow            = NULL;
SDL_Surface *gScreenSurface     = NULL;
SDL_Surface *gXOut              = NULL;

renderer_t  *mainRenderer       = NULL;

char        *loadedObjectName   = "./assets/monkey.obj";

int moveDirection[2] = {0,0}; // {forward, right};

int  init_app();
void close_app();
void init_world();
void destroy_world();
void Input( SDL_Event e );
void UpdateScene( renderer_t *renderer, float delta );

extern void Scene_Destroy( scene_t* );
extern void Renderer_Destroy( renderer_t* );

#if 1

int init_app() {
    int success = TRUE;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not be initialized! SDL_Error: %s\n", SDL_GetError() );
        success = FALSE;
    } else {
        gWindow = SDL_CreateWindow( "Software Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = FALSE;
        } else {
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }
    printf( "Success result is: %i\n", success );
    return success;
}

void close_app() {
    destroy_world();

    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    SDL_Quit();
}

void init_world() {
    obj_model_t *testModelBox = Model_LoadOBJ( loadedObjectName );
    Model_SetPosition( testModelBox, vec3_create(-1.0f, 0.0f, -3.0f));

    obj_model_t *testModelBox1 = Model_CreateBasePlane( "Plane01" );
    Model_SetPosition( testModelBox1, vec3_create(2.0f, 0.0f, -3.0f) );
    Model_AddRotation(testModelBox1, vec3_create(0.0f, 180.0f, 0.0f));
    
    camera_t *Cam = Camera_Init(vec3_create(0.0f, 0.0f, 2.0f),          // Position
                                vec3_create(0.0f, 1.0f, 0.0f),          // Up vector
                                -90.0f,                                 // Yaw
                                0.0f,                                   // Pitch
                                60.0f,                                  // FOV - broken
                                (float)WINDOW_WIDTH/WINDOW_HEIGHT,      // Aspect
                                1.0f,                                   // Near plane
                                100.0f,                                 // Far plane
                                5.0f);                                  // Speed
    
    Scene = Scene_Init( Cam );
    Scene_AddObject( Scene, testModelBox );
    Scene_AddObject( Scene, testModelBox1 );
}

void destroy_world() {
    Scene_Destroy( Scene );
    Renderer_Destroy( mainRenderer );
}

int main(int argc, char* args[]) {
    if( !init_app() ) {
        printf( "Failed to initialize!\n" );
        return 1;
    }

    SDL_Event e;

    init_world();
    mainRenderer = Renderer_Init( Scene, RENDER_STATE_LIT, RENDER_TYPE_SOFTWARE_EDGE );
    
    float rot_val      = 0.0f;
    float frames_slice = 0.0f;
    float total_time   = 0.0f;
    
    float delta = 0.0f;
    
    unsigned int frames_slice_count = 0;
    clock_t curr_time, last_time;
    last_time = clock();

    while ( !quit ) {
        curr_time = clock();
        delta = (float)( curr_time - last_time )/CLOCKS_PER_SEC;
        
        frames_slice += delta;
        total_time += delta;
        frames_slice_count++;

        IH_ProcessInput( e, mainRenderer );
        UpdateScene( mainRenderer, delta );
        Renderer_DrawWorld( mainRenderer, gScreenSurface );
        SDL_UpdateWindowSurface( gWindow );
        
        if(frames_slice > 1.0f){
            //printf("FPS: %u\n", (unsigned int)(1.0f / frames_slice * (float)frames_slice_count));
            frames_slice = 0.0f;
            frames_slice_count = 0;
        }

        last_time = curr_time;
    }

    close_app();

    return 0;
}
#endif

void UpdateScene( renderer_t *renderer, float delta ) {
	obj_model_t *model = Scene_FindObjectByName( renderer->scene, loadedObjectName );
	if( model != NULL )
		Model_AddRotation(model, vec3_create(0.0f, delta, 0.0f));
		
	int f = moveDirection[0]; // 1 - Forward; -1 - Backward;
	int r = moveDirection[1]; // 1 - Right;   -1 - Left;
	
	if(f){
		if(f > 0)
			Camera_ProcMovement( renderer->scene->mainCamera, 1, delta );
		else
			Camera_ProcMovement( renderer->scene->mainCamera, 2, delta );
	}
	if(r){
		if(r > 0)
			Camera_ProcMovement( renderer->scene->mainCamera, 4, delta );
		else
			Camera_ProcMovement( renderer->scene->mainCamera, 3, delta );
	}
	moveDirection[0] = 0;
	moveDirection[1] = 0;
}

void IH_ProcessInput( SDL_Event e, renderer_t *renderer ) {
	
	uint8_t *keystate = SDL_GetKeyboardState(NULL);
	if( keystate[SDL_SCANCODE_W] ) {
		moveDirection[0] = 1;
	}
    if( keystate[SDL_SCANCODE_S] ) {
		moveDirection[0] = -1;
	}
	if( keystate[SDL_SCANCODE_D] ) {
		moveDirection[1] = 1;
	}
    if( keystate[SDL_SCANCODE_A] ) {
		moveDirection[1] = -1;
	}
	
	while( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT ) {
            quit = TRUE;
        } else {
            IH_Handle(e, renderer);
        }
    }
}

void IH_Handle( SDL_Event e, renderer_t *renderer ) {
    switch( e.type ){
        case SDL_KEYDOWN:
            switch( e.key.keysym.sym ) {
                case SDLK_ESCAPE:
                    quit = TRUE;
                    break;
                case SDLK_q:
                    quit = TRUE;
                    break;
                case SDLK_r:
                    Renderer_SwitchRendState( renderer );
                    break;
                default:
                    break;
            }
    }
}
