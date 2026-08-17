#include "fb.h"

#include <stdlib.h>
#include <string.h>

enum { BUF_W = FB_VIEW_W + FB_MARGIN, BUF_H = FB_VIEW_H + FB_MARGIN };

static Pixel *buf[2];
static int back;
static int fine_x, fine_y;
static int origin_x, origin_y;
static int keys[8];

int fb_width(void) { return BUF_W; }
int fb_height(void) { return BUF_H; }
int fb_view_width(void) { return FB_VIEW_W; }
int fb_view_height(void) { return FB_VIEW_H; }
int fb_margin(void) { return FB_MARGIN; }
int fb_fine_x(void) { return fine_x; }
int fb_fine_y(void) { return fine_y; }
int fb_cam_x(void) { return origin_x + fine_x; }
int fb_cam_y(void) { return origin_y + fine_y; }

void fb_key_set(int key, int down)
{
    if (key > 0 && key < 8)
        keys[key] = down;
}

int fb_key_down(int key)
{
    if (key <= 0 || key >= 8)
        return 0;
    return keys[key];
}

int fb_init(void)
{
    size_t n = (size_t)BUF_W * (size_t)BUF_H * sizeof(Pixel);

    buf[0] = calloc(1, n);
    buf[1] = calloc(1, n);
    if (!buf[0] || !buf[1]) {
        free(buf[0]);
        free(buf[1]);
        buf[0] = buf[1] = NULL;
        return 0;
    }
    back = 0;
    fine_x = fine_y = 0;
    origin_x = origin_y = 0;
    return 1;
}

void fb_shutdown(void)
{
    free(buf[0]);
    free(buf[1]);
    buf[0] = buf[1] = NULL;
}

Pixel *fb_back(void)
{
    return buf[back];
}

void fb_put(int x, int y, Pixel color)
{
    if ((unsigned)x >= (unsigned)BUF_W || (unsigned)y >= (unsigned)BUF_H)
        return;
    buf[back][(size_t)y * BUF_W + (size_t)x] = color;
}

static void coarse_x(int dir)
{
    Pixel *p = buf[back];
    int y;

    if (dir > 0) {
        for (y = 0; y < BUF_H; y++)
            memmove(p + (size_t)y * BUF_W, p + (size_t)y * BUF_W + FB_MARGIN,
                    (size_t)(BUF_W - FB_MARGIN) * sizeof(Pixel));
        origin_x += FB_MARGIN;
        fine_x -= FB_MARGIN;
    } else {
        for (y = 0; y < BUF_H; y++)
            memmove(p + (size_t)y * BUF_W + FB_MARGIN, p + (size_t)y * BUF_W,
                    (size_t)(BUF_W - FB_MARGIN) * sizeof(Pixel));
        origin_x -= FB_MARGIN;
        fine_x += FB_MARGIN;
    }
}

static void coarse_y(int dir)
{
    Pixel *p = buf[back];

    if (dir > 0) {
        memmove(p, p + (size_t)FB_MARGIN * BUF_W,
                (size_t)(BUF_H - FB_MARGIN) * BUF_W * sizeof(Pixel));
        origin_y += FB_MARGIN;
        fine_y -= FB_MARGIN;
    } else {
        memmove(p + (size_t)FB_MARGIN * BUF_W, p,
                (size_t)(BUF_H - FB_MARGIN) * BUF_W * sizeof(Pixel));
        origin_y -= FB_MARGIN;
        fine_y += FB_MARGIN;
    }
}

FbCoarse fb_scroll(int dx, int dy)
{
    FbCoarse c = {0, 0};

    while (dx > 0) {
        fine_x++;
        dx--;
        if (fine_x >= FB_MARGIN) {
            coarse_x(1);
            c.coarse_x++;
        }
    }
    while (dx < 0) {
        if (fine_x == 0) {
            coarse_x(-1);
            c.coarse_x--;
        }
        fine_x--;
        dx++;
    }
    while (dy > 0) {
        fine_y++;
        dy--;
        if (fine_y >= FB_MARGIN) {
            coarse_y(1);
            c.coarse_y++;
        }
    }
    while (dy < 0) {
        if (fine_y == 0) {
            coarse_y(-1);
            c.coarse_y--;
        }
        fine_y--;
        dy++;
    }
    return c;
}

void fb_present_copy(void)
{
    /* After present, the new back buffer matches what was just shown. */
    memcpy(buf[back ^ 1], buf[back], (size_t)BUF_W * (size_t)BUF_H * sizeof(Pixel));
    back ^= 1;
}
