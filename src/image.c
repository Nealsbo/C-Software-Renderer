#include "srdefs.h"
#include "image.h"

color_t Color_Init( unsigned char r, unsigned char g, unsigned char b, unsigned char a ){
    color_t c = {r, g, b, a};
    return c;
}

color_t Color_InitFromVec3( vec3 v ) {
	color_t c = {(unsigned char)v.x, (unsigned char)v.y, (unsigned char)v.z, 255};
}

color_t Color_Gray( unsigned char l ) {
    color_t c = {l, l, l, 1};
    return c;
}

color_t Color_ToGray( color_t c ) {
    unsigned char s = ( c.r + c.g + c.b ) / 3;
    color_t r = {s, s, s, c.a};
    return r;
}

void Color_Lerp( color_t res, color_t src1, color_t src2, float a ) {
    res.r = (1 - a) * src1.r + a * src2.r;
    res.g = (1 - a) * src1.g + a * src2.g;
    res.b = (1 - a) * src1.b + a * src2.b;
    res.a = (1 - a) * src1.a + a * src2.a;
}

color_t Color_Intensity( color_t c, float a ) {
	int intens;
    if( a < 0.0f )
        return Color_Init(0, 0, 0, 255);
    if( a > 1.0f )
        return c;
    color_t res = Color_Init( (unsigned char)(c.r * a), (unsigned char)(c.g * a), (unsigned char)(c.b * a), c.a );
    return res;
}

uint32_t Color_ToUInt32( color_t c ) {
    uint32_t r = (uint32_t)c.r;
    uint32_t g = (uint32_t)c.g;
    uint32_t b = (uint32_t)c.b;
    uint32_t a = (uint32_t)c.a;
    return (a << 24) | (r << 16) | (g << 8) | b;
}

uint32_t Color_ToUInt32Fast( color_t c ) {     //broken, indexes reversed
    return *(uint32_t *) &c;
}

void Color_Print( color_t c ) {
    printf( "Color:( r: %d, g: %d, b: %d, a: %d ) \n", c.r, c.g, c.b, c.a );
}

bitmap_t *Bitmap_CreateMap( unsigned int w, unsigned int h ) {
    bitmap_t * new_image;
    new_image = (bitmap_t*)malloc( sizeof(bitmap_t) );
    color_t c = { 0, 0, 0, 0 };

    new_image->width = w;
    new_image->height = h;
    new_image->bitmap = malloc( w * h * sizeof(color_t) );

    Bitmap_FillColor( new_image, c );

    return new_image;
}

bitmap_t *Bitmap_LoadPPM6( const char *fileName ) {
    int BUFLEN = 64;
    int r, i, map_pointer;
    char buf[BUFLEN], *t;
    unsigned char color_buffer[3];
    unsigned int width, height, cdepth;

    FILE     *fp;
    color_t  *map;
    bitmap_t *image;

    if( ( fp = fopen(fileName, "rb") ) == NULL ) {
        printf( "Error: Cannot open file: '%s'!\n", fileName );
        return NULL;
    }

    t = fgets( buf, BUFLEN, fp );

    if ( (t == NULL) || ( strncmp(buf, "P6\n", 3) != 0 ) )
        return NULL;

    do {
        t = fgets( buf, BUFLEN, fp );
        if ( t == NULL )
            return NULL;
    } while ( strncmp(buf, "#", 1) == 0 );

    r = sscanf( buf, "%u %u", &width, &height );
    if ( r < 2 )
        return NULL;

    r = fscanf( fp, "%u", &cdepth );
    if ( (r < 1) || ( cdepth != 255 ) )
        return NULL;

    fseek( fp, 1, SEEK_CUR );

    i = 0;
    map_pointer = width * height;

    image = (bitmap_t *)malloc( sizeof(bitmap_t) );
    image->width  = width;
    image->height = height;
    map = (color_t *)malloc( sizeof(color_t) * width * height );

    while( !feof(fp) ) {
        if( i == map_pointer ) break;
        if( fread( color_buffer, 1, 3, fp ) ) {
            map[i].r = color_buffer[0];
            map[i].g = color_buffer[1];
            map[i].b = color_buffer[2];
            map[i].a = 255;
        } i++;
    }
    image->bitmap = map;
    return image;
}

/*
void Bitmap_SetBitBuffer(bitmap_t *i, uint32_t *buffer, unsigned int w, unsigned int h ) {
    i.bitmap = (color_t*)buffer;
    i.width = width;
    i.height = height;
}*/

void Bitmap_SetPixel( bitmap_t *i, unsigned int x, unsigned int y, color_t c ) {
    if ( x > i->width || x < 0 || y > i->height || y < 0 )
        printf( "Warning: Try to put pixel out of image size! Bitmap size:(%d, %d), Point:(%d, %d)\n", i->width, i->height, x, y );
    else {
        int tmp = y * i->width + x ;
        i->bitmap[ tmp ] = c;
    }
}

color_t Bitmap_GetPixel( bitmap_t *i, unsigned int x, unsigned int y ) {
    color_t col;
    col = i->bitmap[ y * i->width + x ];
    return col;
}

color_t Bitmap_GetPixelUV( bitmap_t *i, vec2 uv ) {
    color_t col;
    unsigned int x = i->width * uv.x;
    unsigned int y = i->height * uv.y;
    col = i->bitmap[ y * i->width + x ];
    return col;
}

void Bitmap_FillColor( bitmap_t *i, color_t c ) {
    unsigned int w, h;
    for( h = 0; h < i->height; h++ ) {
        for( w = 0; w < i->width; w++ ) {
            i->bitmap[ h * i->width + w ] = c;
        }
    }
}

void Bitmap_Save( bitmap_t *i ) {
    FILE *output;
    if ( ( output = fopen( "image.ppm","w" ) ) == NULL )
        printf( "ERROR: Can not create or open file!\n" );
    else{
        fprintf( output, "P3\n%d %d\n255\n", i->width, i->height );
        unsigned int w, h;
        color_t c;
        for( h = 0; h < i->height; h++ ) {
            for( w = 0; w < i->width; w++ ) {
                c = i->bitmap[ h * i->width + w ];
                fprintf( output, "%d %d %d\n", c.r, c.g, c.b );
            }
        }
    }
    fclose(output);
}

void Bitmap_SavePPM3( bitmap_t *i ) {
    FILE *output;
    if ( ( output = fopen( "image.ppm","w" ) ) == NULL )
        printf( "ERROR: Can not create or open file!\n" );
    else{
        fprintf( output, "P3\n%d %d\n255\n", i->width, i->height );
        unsigned int w, h;
        color_t c;
        for( h = 0; h < i->height; h++ ) {
            for( w = 0; w < i->width; w++ ) {
                c = i->bitmap[ h * i->width + w ];
                fprintf( output, "%d %d %d\n", c.r, c.g, c.b );
            }
        }
    }
    fclose(output);
}

void Bitmap_SavePPM6( bitmap_t *i ) {
    FILE *output;
    if ( ( output = fopen( "image1.ppm","wb" ) ) == NULL )
        printf( "ERROR: Can not create or open file!\n" );
    else{
        fprintf( output, "P6\n%d %d\n255\n", i->width, i->height );
        unsigned int w, h;
        color_t c;
        for( h = 0; h < i->height; h++ ) {
            for( w = 0; w < i->width; w++ ) {
                c = i->bitmap[ h * i->width + w ];
                unsigned char wcolor[3];
                wcolor[0] = c.r;
                wcolor[1] = c.g;
                wcolor[2] = c.b;
                fwrite(wcolor, 1, 3, output);
            }
        }
    }
    fclose(output);
}

void Bitmap_PrintInfo( bitmap_t *i ) {
    printf( "Image:\n  Width  = %i;\n  Height = %i;\n", i->width, i->height );
}

void Bitmap_Print( bitmap_t *i ) {
    unsigned int w, h, index;
    for( h = 0; h < i->height; h++ ) {
        for( w = 0; w < i->width; w++ ) {
            index = h * i->width + w;
            printf( "Pos:(%d, %d); r:%d, g:%d, b:%d, a:%d \n", w, h, i->bitmap[index].r, i->bitmap[index].g, i->bitmap[index].b, i->bitmap[index].a  );
        }
    }
    printf( " === \n" );
}

void Bitmap_Free( bitmap_t *i ) {
    free( i->bitmap );
    free( i );
}

//###
//### Bitmap(image) operations
//###
