#ifndef SRDEFS_H_INCLUDED
#define SRDEFS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

#include <SDL2/SDL.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define WINDOW_WIDTH  960
#define WINDOW_HEIGHT 540

#define FPS_LIMIT 60.0

#define CAMERA_SPEED 2.0

#define MAX_NAME_LENGTH 256
#define MAX_TEXT_LENGTH 1024

#define MAX_POLYGON_COUNT 100000

#include "func.h"
#include "data.h"
#include "libmath.h"
#include "image.h"
#include "camera.h"
#include "objloader.h"
#include "render.h"

#endif // SRDEFS_H_INCLUDED
