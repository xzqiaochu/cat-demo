#include <stdint.h>
#include <stdio.h>

#include "tft_utils.h"
#include "tft_ili9341.h"
#include "config.h"

#ifdef TFT_ENABLE
static int font = (int)&SansSerif16x16[0];
#endif

void TFT_Init() {
    printf("Init TFT...");
    MXC_TFT_Init(MXC_SPI0, 1, NULL, NULL);
    MXC_TFT_SetRotation(ROTATE_270);
    MXC_TFT_SetForeGroundColor(WHITE); // set chars to white
    MXC_TFT_ClearScreen();
    printf("\tDone\n");
}

void TFT_Print(int x, int y, char *str, int length)
{
#ifdef TFT_ENABLE
    // fonts id
    text_t text;
    text.data = str;
    text.len = length;

    MXC_TFT_PrintFont(x, y, font, &text, NULL);
#endif
}

void draw_obj_rect(float *xy)
{
#ifdef TFT_ENABLE
    const int r = 253, g = 172, b = 83;

    int x1 = IMG_SIZE_X * xy[0];
    int y1 = IMG_SIZE_Y * xy[1];
    int x2 = IMG_SIZE_X * xy[2];
    int y2 = IMG_SIZE_Y * xy[3];

    // Convert to RGB565
    uint32_t color = ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);

    for (int x = x1; x < x2; ++x) {
        MXC_TFT_WritePixel(x * TFT_SCALE + TFT_OFFSET_X, y1 * TFT_SCALE + TFT_OFFSET_Y, TFT_SCALE, TFT_SCALE, color);
        MXC_TFT_WritePixel(x * TFT_SCALE + TFT_OFFSET_X, y2 * TFT_SCALE + TFT_OFFSET_Y, TFT_SCALE, TFT_SCALE, color);
    }

    for (int y = y1; y < y2; ++y) {
        MXC_TFT_WritePixel(x1 * TFT_SCALE + TFT_OFFSET_X, y * TFT_SCALE + TFT_OFFSET_Y, TFT_SCALE, TFT_SCALE, color);
        MXC_TFT_WritePixel(x2 * TFT_SCALE + TFT_OFFSET_X, y * TFT_SCALE + TFT_OFFSET_Y, TFT_SCALE, TFT_SCALE, color);
    }

    TFT_Print(x1 * TFT_SCALE + TFT_OFFSET_X + THICKNESS, y1 * TFT_SCALE + TFT_OFFSET_Y + THICKNESS, "Cat", 3);
#endif
}
