#include "world.h"

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

Pixel world_at(int wx, int wy)
{
    unsigned h = uhash(wx, wy);
    int n = (int)(h & 31);
    int t, tri, cx, cy, dx, dy, r2;
    unsigned hb;

    t = imod(wx, 96);
    tri = t < 48 ? t : 96 - t;
    if (wy > tri * 2 - 40 && wy < tri * 2 - 35)
        return FB_RGB(40, 78, 98);

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

    if ((h & 0x1ff) == 7)
        return FB_RGB(210, 200, 170);

    if (imod(wx + wy * 2, 23) == 0)
        return FB_RGB(38, 36, 48);

    return FB_RGB(18 + n, 24 + n / 2, 34 + n);
}

void world_fill(void)
{
    int x, y;
    int ox = fb_cam_x() - fb_fine_x();
    int oy = fb_cam_y() - fb_fine_y();

    for (y = 0; y < fb_height(); y++)
        for (x = 0; x < fb_width(); x++)
            fb_put(x, y, world_at(ox + x, oy + y));
}

void world_scroll_keys(float dt)
{
    static float acc, acc_x, acc_y;
    static int prev_x, prev_y;
    int dir_x = fb_key_down(KEY_RIGHT) - fb_key_down(KEY_LEFT);
    int dir_y = fb_key_down(KEY_DOWN) - fb_key_down(KEY_UP);
    int step, step_x, step_y;

    if (dir_x != prev_x) {
        acc_x = 0;
        acc = 0;
        prev_x = dir_x;
    }
    if (dir_y != prev_y) {
        acc_y = 0;
        acc = 0;
        prev_y = dir_y;
    }
    if (!dir_x)
        acc_x = 0;
    if (!dir_y)
        acc_y = 0;
    if (!dir_x || !dir_y)
        acc = 0;

    if (dir_x && dir_y) {
        acc += SCROLL_PX_S * dt;
        step = (int)acc;
        acc -= (float)step;
        if (step)
            fb_scroll(dir_x * step, dir_y * step);
    } else {
        acc_x += (float)dir_x * SCROLL_PX_S * dt;
        acc_y += (float)dir_y * SCROLL_PX_S * dt;
        step_x = (int)acc_x;
        step_y = (int)acc_y;
        acc_x -= (float)step_x;
        acc_y -= (float)step_y;
        if (step_x || step_y)
            fb_scroll(step_x, step_y);
    }
}
