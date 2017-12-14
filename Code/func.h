#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))

#define SWAP(x, y, t) {t temp = x; x = y; y = temp;} while (0)          //Int and float swap macro
#define SWAP_STRUCT(x, y, t) {t temp = *x; *x = *y; *y = temp;}         //Struct type swap macro

#endif // FUNC_H_INCLUDED
