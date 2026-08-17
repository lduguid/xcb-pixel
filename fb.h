#ifndef FB_H
#define FB_H

#include <stdint.h>

/* Minimal pixel harness. The only drawing primitive is fb_put().
 * Lines, rects, sprites, text — you write those with pixels.
 *
 * The bitmap is VIEW + MARGIN on each axis. The window shows a VIEW-sized
 * slice at (fine_x, fine_y). fb_scroll moves that slice one pixel at a time
 * (fine scroll). After MARGIN pixels, the bitmap is memmove'd by MARGIN and
 * fine scroll wraps — the same coarse/fine split as a C64 or Amiga.
 * Newly exposed strips are undefined; redraw them if you keep pixels around.
 *
 * Double-buffered: fb_put writes the back buffer. fb_swap presents the
 * visible slice and copies it to the new back buffer. */

typedef uint32_t Pixel; /* 0x00RRGGBB */

enum {
    FB_VIEW_W = 320,
    FB_VIEW_H = 200,
    FB_MARGIN = 8
};

#define FB_RGB(r, g, b) ((Pixel)(((r) << 16) | ((g) << 8) | (b)))

int fb_width(void);       /* VIEW_W + MARGIN */
int fb_height(void);      /* VIEW_H + MARGIN */
int fb_view_width(void);
int fb_view_height(void);
int fb_margin(void);

/* Fine-scroll origin of the visible slice inside the bitmap, 0 .. MARGIN-1. */
int fb_fine_x(void);
int fb_fine_y(void);

/* World coordinate of the visible top-left pixel. */
int fb_cam_x(void);
int fb_cam_y(void);

void fb_put(int x, int y, Pixel color);

void fb_swap(void);

/* Positive dx looks right, positive dy looks down. */
typedef struct {
    int coarse_x; /* -1, 0, +1: bitmap shifted by ±MARGIN on x */
    int coarse_y;
} FbCoarse;

FbCoarse fb_scroll(int dx, int dy);

enum {
    KEY_LEFT = 1,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_ESC,
    KEY_Q
};

int fb_key_down(int key);

/* Platform. tick(dt) is called each frame with elapsed seconds. */
int fb_main(void (*tick)(float dt));

#endif
