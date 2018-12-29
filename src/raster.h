#ifndef RASTER_H_INCLUDED
#define RASTER_H_INCLUDED

#include "srdefs.h"

void Raster_DrawLine         ( vertex_t v0, vertex_t v1, SDL_Surface *image, color_t color);
void Raster_DrawTriangle     ( vertex_t *v, SDL_Surface *image, obj_model_t *model );

#endif // RASTER_H_INCLUDED
