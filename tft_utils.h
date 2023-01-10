#include "tft_ili9341.h"

#include "example_config.h"

#define THICKNESS 4

void TFT_Print(char *str, int x, int y, int font, int length);
void draw_obj_rect(float *xy, int class_idx, uint32_t w, uint32_t h, uint8_t scale);
