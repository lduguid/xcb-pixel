#ifndef BOBS_H
#define BOBS_H

#include "fb.h"

enum { BOB_MAX = 12 };

typedef struct {
    float x, y, vx, vy;
    int r;
    Pixel color;
} Bob;

void bobs_init(Bob *b, int n, float x0, float y0, float x1, float y1);
void bobs_update(Bob *b, int n, float dt, float x0, float y0, float x1, float y1);
void bobs_draw(const Bob *b, int n, int origin_x, int origin_y);

#endif
