#ifndef __PROCESS_H__
#define __PROCESS_H_

#include <stdint.h>

#define SQUARE(x) ((x) * (x))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define NUM_ARS 4
#define NUM_SCALES 4
#define NUM_CLASSES 2 // cat and background

#define LOC_DIM 4 //(x, y, w, h) or (x1, y1, x2, y2)

#define NUM_PRIORS_PER_AR 425
#define NUM_PRIORS NUM_PRIORS_PER_AR * NUM_ARS

void get_priors(void);
void nms(void);
void get_cxcy(float *cxcy, int prior_idx);
void gcxgcy_to_cxcy(float *cxcy, int prior_idx, float *priors_cxcy);
void cxcy_to_xy(float *xy, float *cxcy);
void localize_objects(void);

#endif // __PROCESS_H__
