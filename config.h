#ifndef __CONFIG_H__
#define __CONFIG_H__

// #define USE_SAMPLEDATA

// CNN
#define MAX_PRIORS 100
#define MIN_CLASS_SCORE 47857 // 65536 * 70%
#define MAX_ALLOWED_OVERLAP 0.3
#define XY_MIN 0.05f
#define XY_MAX 0.95f

// MOTOR
#define MIN_CONF 0.7f
#define MIN_AREA 0.2f

// CAMERA
#define CAMERA_FREQ (10 * 1000 * 1000)
#define IMG_SIZE_X 74
#define IMG_SIZE_Y 74

// TFT
#define TFT_ENABLE
#define TFT_BUFF_SIZE 50
#define TFT_SCALE 2
#define TFT_OFFSET_X 86 
#define TFT_OFFSET_Y 21

#endif // __CONFIG_H__
