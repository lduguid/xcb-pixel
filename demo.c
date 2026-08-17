/* User program: only fb_put() to draw. Everything else is loops of pixels.
 * Arrows (or WASD) fine-scroll; after FB_MARGIN pixels the harness coarse-shifts. */

#include "fb.h"

enum { SCROLL_PX_S = 140 };

static int imod(int v, int m)
{
    int r = v % m;
    return r < 0 ? r + m : r;
}

static int icell(int v, int m)
{
    int q = v / m;
    if (v < 0 && v % m)
        q--;
    return q;
}

static unsigned uhash(int x, int y)
{
    unsigned h = (unsigned)x * 374761393u + (unsigned)y * 668265263u;
    h = (h ^ (h >> 13)) * 1274126177u;
    return h ^ (h >> 16);
}

/* Irregular world: noise, a meander, and scattered blobs. No large axis-aligned
 * checker squares, so a scanline/tear or moiré is easier to judge. */
static Pixel world_at(int wx, int wy)
{
    unsigned h = uhash(wx, wy);
    int n = (int)(h & 31);
    int t, tri, cx, cy, dx, dy, r2;
    unsigned hb;

    /* meandering band (period 96, not a power of two) */
    t = imod(wx, 96);
    tri = t < 48 ? t : 96 - t;
    if (wy > tri * 2 - 40 && wy < tri * 2 - 35)
        return FB_RGB(40, 78, 98);

    /* blob in a 53x41 cell, center jittered so they are not on a grid */
    hb = uhash(icell(wx, 53), icell(wy, 41));
    cx = icell(wx, 53) * 53 + 12 + (int)(hb % 29);
    cy = icell(wy, 41) * 41 + 8 + (int)((hb >> 8) % 25);
    dx = wx - cx;
    dy = wy - cy;
    r2 = dx * dx + dy * dy;
    if (r2 < 64)
        return FB_RGB(88, 62, 50);
    if (r2 < 81)
        return FB_RGB(110, 80, 62);

    /* sparse specks */
    if ((h & 0x1ff) == 7)
        return FB_RGB(210, 200, 170);

    /* faint diagonal grain, period 23 */
    if (imod(wx + wy * 2, 23) == 0)
        return FB_RGB(38, 36, 48);

    return FB_RGB(18 + n, 24 + n / 2, 34 + n);
}

static void put_all_world(void)
{
    int x, y;
    int ox = fb_cam_x() - fb_fine_x();
    int oy = fb_cam_y() - fb_fine_y();

    for (y = 0; y < fb_height(); y++)
        for (x = 0; x < fb_width(); x++)
            fb_put(x, y, world_at(ox + x, oy + y));
}

static void tick(float dt)
{
    static float acc, acc_x, acc_y;
    int dir_x = fb_key_down(KEY_RIGHT) - fb_key_down(KEY_LEFT);
    int dir_y = fb_key_down(KEY_DOWN) - fb_key_down(KEY_UP);
    int cx, cy, i, step, step_x, step_y;

    /* Independent remainders on X and Y make a diagonal stair-step (2px on one
     * axis, 3px on the other). One accumulator keeps diagonal steps equal. */
    if (dir_x && dir_y) {
        acc_x = acc_y = 0;
        acc += SCROLL_PX_S * dt;
        step = (int)acc;
        acc -= (float)step;
        if (step)
            fb_scroll(dir_x * step, dir_y * step);
    } else {
        acc = 0;
        acc_x += (float)dir_x * SCROLL_PX_S * dt;
        acc_y += (float)dir_y * SCROLL_PX_S * dt;
        step_x = (int)acc_x;
        step_y = (int)acc_y;
        acc_x -= (float)step_x;
        acc_y -= (float)step_y;
        if (step_x || step_y)
            fb_scroll(step_x, step_y);
    }

    /* Rebuild the bitmap from pixels. Replace this with your own primitives. */
    put_all_world();

    cx = fb_fine_x() + fb_view_width() / 2;
    cy = fb_fine_y() + fb_view_height() / 2;
    for (i = -2; i <= 2; i++) {
        fb_put(cx + i, cy, FB_RGB(255, 220, 80));
        fb_put(cx, cy + i, FB_RGB(255, 220, 80));
    }
    fb_swap();
}

int main(void)
{
    return fb_main(tick);
}
