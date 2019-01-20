#include "srdefs.h"
#include "application.h"

const int SCREEN_TICK_PER_FRAME = 1000 / FPS_LIMIT;

int quit = FALSE;

float cam_speed = 1.0f;
float elapsed_time = 0.0f;

scene_t     *Scene;
camera_t    *Cam;

SDL_Window  *gWindow;
SDL_Surface *gScreenSurface;
SDL_Surface *gXOut;

renderer_t  *mainRenderer;

char        *loadedObjectName   = "./assets/monkey.obj";

int moveDirection[2] = {0,0}; // {forward, right};

static void SetupScene() {
	printf( "Scene Setup: Start\n" );
    //obj_model_t *testModel0 = Model_LoadOBJ( loadedObjectName );
    obj_model_t *testModel0 = Model_CreateBasePlane( "Plane01" );
    Model_SetPosition( testModel0, vec3_create( 0.0f, 0.0f, -2.0f ) );
    Model_SetRotation( testModel0, vec3_create( 0.0f, to_radians( 180.0f ), 0.0f ) );
    /*
    obj_model_t *testModel1 = Model_CreateBasePlane( "Plane01" );
    Model_SetRotation( testModel1, vec3_create( to_radians( -90.0f ), to_radians( 180.0f ), 0.0f ) );
    Model_SetPosition( testModel1, vec3_create( 0.0f, -2.0f, 0.0f ) );
    Model_SetScale( testModel1, vec3_create( 5.0f, 5.0f, 5.0f ) );
    */
    camera_t *Cam = Camera_Init(vec3_create( 0.0f, 0.0f, 2.0f ),          // Position
                                vec3_create( 0.0f, 1.0f, 0.0f ),          // Up vector
                                -90.0f,                                 // Yaw
                                0.0f,                                   // Pitch
                                60.0f,                                  // FOV - broken
                                (float)WINDOW_WIDTH/WINDOW_HEIGHT,      // Aspect
                                0.5f,                                   // Near plane
                                100.0f,                                 // Far plane
                                5.0f);                                  // Speed
    
    Scene = Scene_Init( Cam );
    Scene_SetDirectLight( Scene, vec3_create( 0.0f, 3.0f, 5.0f ), vec3_create( 1.0f, 0.5f, 0.0f ) );
    Scene_SetAmbientLight( Scene, vec3_create( 0.0f, 0.5f, 1.0f ) );
    Scene_AddObject( Scene, testModel0 );
    //Scene_AddObject( Scene, testModel1 );
}

static void SceneCleanUp() {
    Scene_Destroy( Scene );
    Renderer_Destroy( mainRenderer );
}

int Application_Init() {	
	printf( "Application Init: Start\n");
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
	printf( "Application Init: Success\n");
	return success;
}

void Application_Run() {
	printf( "Application Run: Start\n");
    SDL_Event e;

    SetupScene( Scene );
    mainRenderer = Renderer_Init( Scene, RENDER_STATE_LIT, RENDER_TYPE_SOFTWARE );
    
    float frames_slice = 0.0f;
    float delta = 0.0f;
    
    unsigned int frames_slice_count = 0;
    clock_t curr_time, last_time;
    last_time = clock();
    
	printf( "Application Run: Loop\n");
    while ( !quit ) {
        curr_time = clock();
        delta = (float)( curr_time - last_time )/CLOCKS_PER_SEC;
        frames_slice += delta;
        elapsed_time += delta;
        frames_slice_count++;
        
        IH_ProcessInput( e, mainRenderer );
        UpdateScene( mainRenderer, delta );
        Renderer_DrawWorld( mainRenderer, gScreenSurface );
        SDL_UpdateWindowSurface( gWindow );
        
        printf("%f\n", elapsed_time);
        if(frames_slice > 1.0f){
            printf("FPS: %u\n", (unsigned int)(1.0f / frames_slice * (float)frames_slice_count));
            frames_slice = 0.0f;
            frames_slice_count = 0;
        }

        if( quit ) {
            break;
        }
        last_time = curr_time;
    }

    return 0;
}

void Application_Close() {
	printf( "Application Close\n");
	SceneCleanUp();
	
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	
	SDL_Quit();
}

void UpdateScene( renderer_t *renderer, float delta ) {
	//vec3 light_new_pos = vec3_create( 5.0f * cosf(elapsed_time * 2.0f), renderer->scene->directLight.y, renderer->scene->directLight.z );
	//vec3 light_new_pos = vec3_byMat4( renderer->scene->directLight, mat4_roty(delta * 1.0f) );
	//renderer->scene->directLight = light_new_pos;
	
	obj_model_t *model = Scene_FindObjectByName( renderer->scene, "Plane01" );
	if( model != NULL )
		Model_AddRotation( model, vec3_create( 0.0f, delta, 0.0f ) );
	
	
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
			Camera_ProcMovement( renderer->scene->mainCamera, 3, delta );
		else
			Camera_ProcMovement( renderer->scene->mainCamera, 4, delta );
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
