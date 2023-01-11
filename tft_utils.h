#include "tft_ili9341.h"
#include "example_config.h"

#define THICKNESS 4

void TFT_Init();
void TFT_Print(int x, int y, char *str, int length);
void draw_obj_rect(float *xy);
