#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "mxc.h"
#include "mxc_device.h"
#include "board.h"
#include "mxc_delay.h"
#include "rtc.h"
#include "uart.h"
#include "tft_utils.h"
#include "example_config.h"
#include "post_process.h"

#ifdef TFT_ENABLE
static int font = (int)&SansSerif16x16[0];

static text_t label_text[] = {
    // info
    { (char *)"Cat", 3 },  { (char *)"Two", 3 },  { (char *)"Three", 5 }, { (char *)"Four", 4 },
    { (char *)"Five", 4 }, { (char *)"Six", 3 },  { (char *)"Seven", 5 }, { (char *)"Eight", 5 },
    { (char *)"Nine", 4 }, { (char *)"Zero", 4 },
};
#endif

void TFT_Print(char *str, int x, int y, int font, int length)
{
    // fonts id
    text_t text;
    text.data = str;
    text.len = length;

    MXC_TFT_PrintFont(x, y, font, &text, NULL);
}

void draw_obj_rect(float *xy, int class_idx, uint32_t w, uint32_t h, uint8_t scale)
{
#ifdef TFT_ENABLE
    int r = 0, g = 0, b = 0;
    uint32_t color;

    if (class_idx == 0) {
        r = 253;
        g = 172;
        b = 83;
    } else if (class_idx == 1) {
        r = 155;
        g = 183;
        b = 212;
    } else if (class_idx == 2) {
        r = 181;
        g = 90;
        b = 48;
    } else if (class_idx == 3) {
        r = 245;
        g = 223;
        b = 77;
    } else if (class_idx == 4) {
        r = 48;
        g = 120;
        b = 180;
    } else if (class_idx == 5) {
        r = 160;
        g = 218;
        b = 169;
    } else if (class_idx == 6) {
        r = 233;
        g = 137;
        b = 126;
    } else if (class_idx == 7) {
        r = 0;
        g = 182;
        b = 148;
    } else if (class_idx == 8) {
        r = 147;
        g = 105;
        b = 168;
    } else if (class_idx == 9) {
        r = 210;
        g = 56;
        b = 108;
    }

    int x1 = w * xy[0];
    int y1 = h * xy[1];
    int x2 = w * xy[2];
    int y2 = h * xy[3];
    int x, y;

    // Convert to RGB565
    color = ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);

    for (x = x1; x < x2; ++x) {
        MXC_TFT_WritePixel(x * scale + IMG_OFFSET_X, y1 * scale + IMG_OFFSET_Y, scale, scale, color);
        MXC_TFT_WritePixel(x * scale + IMG_OFFSET_X, y2 * scale + IMG_OFFSET_Y, scale, scale, color);
    }

    for (y = y1; y < y2; ++y) {
        MXC_TFT_WritePixel(x1 * scale + IMG_OFFSET_X, y * scale + IMG_OFFSET_Y, scale, scale, color);
        MXC_TFT_WritePixel(x2 * scale + IMG_OFFSET_X, y * scale + IMG_OFFSET_Y, scale, scale, color);
    }

    MXC_TFT_PrintFont(x1 * scale + IMG_OFFSET_X + THICKNESS, y1 * scale + IMG_OFFSET_Y + THICKNESS, font, &label_text[class_idx],
                      NULL);
#endif
}
