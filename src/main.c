#include "srdefs.h"

const int SCREEN_TICK_PER_FRAME = 1000 / FPS_LIMIT;

int quit = FALSE;

scene_t         *Scene              = NULL;
camera_t        *Cam                = NULL;

SDL_Window      *gWindow            = NULL;
SDL_Surface     *gScreenSurface     = NULL;
SDL_Surface     *gXOut              = NULL;

renderContext_t *mainRenderContext  = NULL;
renderInfo_t    *mainRenderer       = NULL;

int  init_app();
void close_app();
void putpixel(SDL_Surface *surface, int x, int y, uint32_t pixel);
void init_world();
void destroy_world();

extern void Scene_Destroy( scene_t* );
extern void Render_Destroy( renderInfo_t*, renderContext_t* );

#if 1

int init_app(){
    int success = TRUE;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
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
    printf("Success result is: %i\n", success);
    return success;
}

void close_app(){
    destroy_world();

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

void init_world(){
    obj_model_t *testModelBox = Model_LoadOBJ("./assets/monkey.obj");
    //obj_model_t *testModelBox = Model_CreateBaseBox("Box01");
    Model_SetPosition(testModelBox, vec3_create(-1.0, 0.0, -6.0));

    obj_model_t *testModelBox1 = Model_CreateBasePlane("Plane01");
    //obj_model_t *testModelBox1 = Model_CreateBaseTriangle("Triangle01");
    Model_SetPosition(testModelBox1, vec3_create(2.0, 0.0, -6.0));
    
    camera_t *Cam = Camera_Init(vec3_create(0.0f, 0.0f, 0.0f),          //Position
                                vec3_create(0.0f, 0.0f, -1.0f),         //Direction
                                vec3_create(0.0f, 1.0f, 0.0f),          //Up vector
                                120.0f,                                  //FOV - broken
                                (float)WINDOW_WIDTH/WINDOW_HEIGHT,      //Aspect
                                1.0f,                                   //Near plane
                                100.0f);                                //Far plane
    
    Scene = Scene_Init(Cam);
    Scene_AddObject(Scene, testModelBox);
    Scene_AddObject(Scene, testModelBox1);
}

void destroy_world(){
    Scene_Destroy(Scene);
    Render_Destroy(mainRenderer, mainRenderContext);
}

int main(int argc, char* args[])
{
	if( !init_app() ){
		printf( "Failed to initialize!\n" );
		return 1;
	}

	SDL_Event e;

	init_world();
    mainRenderer = Render_Init(mainRenderContext, RENDER_STATE_LIT);

    float rot_val = 0.0;
    unsigned int fps = 0;
    unsigned int ticks, ticksDiff;
	unsigned int lastTicks = SDL_GetTicks();
	float cam_speed = 0.0f;

    while (!quit){

        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT ){
                quit = TRUE;
            } else if( e.type == SDL_KEYDOWN ) {
                switch( e.key.keysym.sym ) {
                    case SDLK_ESCAPE:
                        quit = TRUE;
                        break;
                    case SDLK_q:
						quit = TRUE;
                        break;
                    case SDLK_r:
                        Render_SwitchRenderState(mainRenderer);
                        break;
                    case SDLK_w:
                        Camera_PerfMovement(Scene->mainCamera, 1, cam_speed);
                        break;
                    case SDLK_s:
                        Camera_PerfMovement(Scene->mainCamera, 2, cam_speed);
                        break;
                    case SDLK_a:
                        Camera_PerfMovement(Scene->mainCamera, 3, cam_speed);
                        break;
                    case SDLK_d:
                        Camera_PerfMovement(Scene->mainCamera, 4, cam_speed);
                        break;
                    default:
                        break;
                }
            }
        }

        SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x00, 0x00 ) );
        Render_ClearZBuffer(mainRenderer);
        Render_DrawWorld(Scene, mainRenderer, gScreenSurface, rot_val);
        SDL_UpdateWindowSurface( gWindow );

        ticks = SDL_GetTicks();
		ticksDiff = ticks - lastTicks;

		if(ticksDiff == 0)
			continue;

		lastTicks = ticks;
		fps = 1000 / ticksDiff;
		rot_val += (float)ticksDiff / 2000.f;
		cam_speed = ticks / 5000.0f;

		printf("FPS: %u\t\r", fps);

		if( ticksDiff < SCREEN_TICK_PER_FRAME ){
            SDL_Delay( SCREEN_TICK_PER_FRAME - ticksDiff );
		}
		SDL_Delay( 10.f );
		if( quit ){
            break;
        }
    }

	close_app();

	return 0;
}
#endif

#if 0
int main(int argc, char* args[]){

	return 0;
}
#endif
