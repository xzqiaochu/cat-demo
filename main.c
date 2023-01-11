#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "mxc.h"
#include "icc.h"
#include "dma.h"
#include "led.h"
#include "camera.h"

#include "cnn.h"
#include "sampledata.h"

#include "process.h"
#include "tft.h"
#include "config.h"

volatile uint32_t cnn_time; // Stopwatch

// 3-channel 74x74 data input (16428 bytes total / 5476 bytes per channel):
// HWC 74x74, channels 0 to 2
#ifdef USE_SAMPLEDATA //Sample DATA
static uint32_t input_buffer[] = SAMPLE_INPUT_0;
uint32_t *input = input_buffer;
#endif

void load_input(void)
{
#ifdef USE_SAMPLEDATA
    // This function loads the sample data input -- replace with actual data
    memcpy32((uint32_t *)0x50402000, input, IMG_SIZE_X * IMG_SIZE_Y);
#else // Camera
    uint8_t *frame_buffer;
    uint8_t *buffer;
    uint32_t imgLen;
    uint32_t w, h, x, y;
    uint8_t r, g, b;
    uint32_t *cnn_mem = (uint32_t *)0x50402000;
    uint32_t color;
#ifdef TFT_ENABLE
    uint8_t image[IMG_SIZE_X * TFT_SCALE][IMG_SIZE_Y * TFT_SCALE][2];
#endif // TFT_ENABLE

    camera_start_capture_image();

    while (!camera_is_image_rcv()) {}

    camera_get_image(&frame_buffer, &imgLen, &w, &h);
    buffer = frame_buffer;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            r = *buffer++;
            g = *buffer++;
            b = *buffer++;
            buffer++; // skip msb=0x00
            // change the range from [0,255] to [-128,127] and store in buffer for CNN
            *cnn_mem++ = ((b << 16) | (g << 8) | r) ^ 0x00808080;

            // display on TFT
#ifdef TFT_ENABLE
            // Convert to RGB565
            color = ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
            for (uint8_t i = 0; i < TFT_SCALE; i++) {
                for (uint8_t j = 0; j < TFT_SCALE; j++) {
                    image[x*TFT_SCALE+i][y*TFT_SCALE+j][0] = color >> 8;
                    image[x*TFT_SCALE+i][y*TFT_SCALE+j][1] = color & 0xff;
                }
            }
#endif // TFT_ENABLE
        }
    }
#ifdef TFT_ENABLE
    MXC_TFT_ShowImageCameraRGB565(TFT_OFFSET_X, TFT_OFFSET_Y, (uint8_t *)image, IMG_SIZE_X * TFT_SCALE, IMG_SIZE_Y * TFT_SCALE);
#endif // TFT_ENABLE
#endif
}

int main(void)
{
    // Wait for PMIC 1.8V to become available, about 180ms after power up.
    MXC_Delay(200000);
    
    printf("\n\nCat Detection Demo\n");

    /* Enable cache & Switch clock. */
    printf("Init System...");
    
    MXC_ICC_Enable(MXC_ICC0); // Enable cache

    // Switch to 100 MHz clock
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

    // DO NOT DELETE THIS LINE:
    MXC_Delay(SEC(2)); // Let debugger interrupt if needed

    printf("\tDone\n");
    
    /* Initialize TFT display. */
    char buff[TFT_BUFF_SIZE];
    TFT_Init();

#if !defined(USE_SAMPLEDATA)
    /* Initialize camera. */
    printf("Init Camera...");

    Camera_Power(POWER_ON);

    // Initialize DMA for camera interface
    MXC_DMA_Init();
    int dma_channel = MXC_DMA_AcquireChannel();

    camera_init(CAMERA_FREQ);

    int ret = camera_setup(IMG_SIZE_X, IMG_SIZE_Y, PIXFORMAT_RGB888, FIFO_THREE_BYTE, USE_DMA, dma_channel);

    if (ret != STATUS_OK) {
        printf("\tError returned from setting up camera. Error %d\n", ret);
        return -1;
    } else {
        printf("\tDone\n");
    }
#else // !defined(USE_SAMPLEDATA)
    printf("Using Sample Data!\n");
#endif // !defined(USE_SAMPLEDATA)

    /* Initialize CNN */
    printf("Init CNN...");
    // Configure P2.5, turn on the CNN Boost
    mxc_gpio_cfg_t gpio_out;
    gpio_out.port = MXC_GPIO2;
    gpio_out.mask = MXC_GPIO_PIN_5;
    gpio_out.pad = MXC_GPIO_PAD_NONE;
    gpio_out.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_out);
    MXC_GPIO_OutSet(gpio_out.port, gpio_out.mask);
    // Enable peripheral, enable CNN interrupt, turn on CNN clock
    // CNN clock: 50 MHz div 1
    cnn_enable(MXC_S_GCR_PCLKDIV_CNNCLKSEL_PCLK, MXC_S_GCR_PCLKDIV_CNNCLKDIV_DIV1);
    cnn_init(); // Bring state machine into consistent state
    cnn_load_weights(); // Load kernels
    cnn_load_bias();
    cnn_configure(); // Configure state machine
    printf("\tDone\n");

    while (1) {
        load_input(); // Load data input
        LED_On(LED1);
        cnn_start(); // Start CNN processing

        while (cnn_time == 0) {
            __WFI(); // Wait for CNN
        }

        LED_Off(LED1);
        get_priors();
        localize_objects();

        printf("CNN time: %.3f ms\n\n", (float)cnn_time / 1000);
        TFT_Print(5, 210, buff, snprintf(buff, sizeof(buff), "CNN Time: %.3f ms", (float)cnn_time / 1000));
        
        // MXC_Delay(SEC(1));
    }

    return 0;
}
