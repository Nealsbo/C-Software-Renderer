#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

//################
//### Typedefs ###
//################

typedef struct {
    unsigned char r, g, b, a;
} color_t;

typedef struct {
    unsigned int width;
    unsigned int height;
    color_t      *bitmap;
} bitmap_t;

//#######################
//### Color Functions ###
//#######################

color_t   Color_Init        ( unsigned char r, unsigned char g, unsigned char b, unsigned char a );
void      Color_Screen      ( color_t res, color_t src1, color_t src2 );
void      Color_MakeGray    ( color_t c );
void      Color_Lerp        ( color_t res, color_t src1, color_t src2, float a );
uint32_t  Color_ToUInt32    ( color_t c );
uint32_t  Color_ToUInt32Fast( color_t c );

void      Color_Print       ( color_t c );

//###############################
//### Bitmap(Image) Functions ###
//###############################

bitmap_t *Bitmap_CreateMap   ( unsigned int w, unsigned int h );
bitmap_t *Bitmap_LoadBMP     ( const char *fileName );      //TODO
bitmap_t *Bitmap_LoadTGA     ( const char *fileName );      //TODO
bitmap_t *Bitmap_LoadPPM6    ( const char *fileName );
void      Bitmap_SetBitBuffer( bitmap_t *i, uint32_t *buffer, unsigned int w, unsigned int h );
void      Bitmap_SetPixel    ( bitmap_t *i, unsigned int x, unsigned int y, color_t c );
color_t   Bitmap_GetPixel    ( bitmap_t *i, unsigned int x, unsigned int y );
void      Bitmap_FillColor   ( bitmap_t *i, color_t c );
void      Bitmap_SetOnTop    ( bitmap_t *i, bitmap_t *src, unsigned int x, unsigned int y );
void      Bitmap_Save        ( bitmap_t *i );
void      Bitmap_SavePPM3    ( bitmap_t *i );
void      Bitmap_SavePPM6    ( bitmap_t *i );
void      Bitmap_Print       ( bitmap_t *i );
void      Bitmap_Free        ( bitmap_t *i );

void      Bitmap_BlurMap     ( bitmap_t *i, float value );      //TODO

#endif // IMAGE_H_INCLUDED
