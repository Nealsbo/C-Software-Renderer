#ifndef RASTER_H_INCLUDED
#define RASTER_H_INCLUDED

#include "srdefs.h"

void Raster_DrawLine         ( vec4 v0, vec4 v1, SDL_Surface *image, color_t color);
void Raster_DrawTriangle     ( vec4 *v, SDL_Surface *image, obj_model_t *model, shader_t *shader );

#endif // RASTER_H_INCLUDED
