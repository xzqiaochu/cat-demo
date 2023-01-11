#ifndef __TFT_H__
#define __TFT_H__

#include <stdint.h>

void TFT_Init();
void TFT_Print(int x, int y, char *str, int length);
void draw_obj_rect(float *xy);

#endif // __TFT_H__
