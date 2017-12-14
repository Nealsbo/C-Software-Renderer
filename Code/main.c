#include "srdefs.h"

int quit = FALSE;

scene_t         *Scene              = NULL;
camera_t        *Cam                = NULL;

SDL_Window      *gWindow            = NULL;
SDL_Surface     *gScreenSurface     = NULL;
SDL_Surface     *gXOut              = NULL;

renderContext_t *mainRenderContext  = NULL;
renderInfo_t    *mainRender         = NULL;

void init_world();
void destroy_world();

int  init();
void close();
void putpixel(SDL_Surface *surface, int x, int y, uint32_t pixel);
void init_world();

#if 1

int init(){
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
    return success;
}

void close(){
    destroy_world();

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_Quit();
}

void init_world(){
    obj_model_t *testModelBox = Model_LoadOBJ("monkey.obj");
    camera_t *Cam = Camera_Init(vec3_create(0.0f, 0.0f, 0.0f),          //Position
                                vec3_create(0.0f, 0.0f, -1.0f),         //Direction
                                vec3_create(0.0f, 1.0f, 0.0f),          //Up vector
                                120.0f,                                 //FOV - broken
                                (float)WINDOW_WIDTH/WINDOW_HEIGHT,      //Aspect
                                5.0f,                                   //Near plane
                                500.0f);                                //Far plane

    Scene = Scene_Init(Cam);
    Scene_AddObject(Scene, testModelBox);
}

void destroy_world(){
    Scene_Destroy(Scene);
    Render_Destroy(mainRender, mainRenderContext);
}

int main(int argc, char* args[])
{
	if( !init() ){
		printf( "Failed to initialize!\n" );
		return 1;
	}

	SDL_Event e;

	init_world();
    mainRender = Render_Init(mainRenderContext, RENDER_STATE_WIREFRAME);

    float rot_val = 0.0;
    unsigned int fps = 0;
    unsigned int ticks, ticksDiff;
	unsigned int lastTicks = SDL_GetTicks();

    while (!quit){

        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT ){
                quit = TRUE;
            } else if( e.type == SDL_KEYDOWN ) {
                switch( e.key.keysym.sym ) {
                    case SDLK_ESCAPE:
                        quit = TRUE;
                        break;
                    case SDLK_r:
                        Render_SwitchRenderState(mainRender);
                    default:
                        break;
                }
            }
        }

        SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x00, 0x00 ) );
        Render_DrawWorld(Scene, mainRender, gScreenSurface, rot_val);
        SDL_UpdateWindowSurface( gWindow );

        ticks = SDL_GetTicks();
		ticksDiff = ticks - lastTicks;

		if(ticksDiff == 0)
			continue;

		lastTicks = ticks;
		fps = 1000 / ticksDiff;
		rot_val += (float)ticksDiff / 1000.f;

		printf("FPS: %u\t\r", fps);

        SDL_Delay( 10 );
    }

	close();

	return 0;
}
#endif

#if 0
int main(int argc, char* args[]){

	return 0;
}
#endif
