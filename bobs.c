#include "bobs.h"

static const Pixel bob_colors[] = {
    FB_RGB(255, 80, 80),  FB_RGB(80, 220, 120), FB_RGB(80, 160, 255),
    FB_RGB(255, 200, 60), FB_RGB(220, 90, 200), FB_RGB(90, 230, 230),
    FB_RGB(255, 140, 60), FB_RGB(180, 255, 90), FB_RGB(160, 120, 255),
    FB_RGB(255, 90, 140), FB_RGB(90, 255, 180), FB_RGB(255, 255, 120),
};

void bobs_init(Bob *b, int n, float x0, float y0, float x1, float y1)
{
    int i;
    float w = x1 - x0, h = y1 - y0;

    for (i = 0; i < n; i++) {
        int r = 3 + (i % 5);
        b[i].r = r;
        b[i].x = x0 + r + 8 + (i * 23) % (int)(w - 2 * r - 16);
        b[i].y = y0 + r + 6 + (i * 17) % (int)(h - 2 * r - 12);
        b[i].vx = (i & 1) ? 40.0f + i * 7.0f : -(36.0f + i * 5.0f);
        b[i].vy = (i & 2) ? 32.0f + i * 4.0f : -(28.0f + i * 6.0f);
        b[i].color = bob_colors[i % (int)(sizeof bob_colors / sizeof bob_colors[0])];
    }
}

void bobs_update(Bob *b, int n, float dt, float x0, float y0, float x1, float y1)
{
    int i;

    for (i = 0; i < n; i++) {
        float r = (float)b[i].r;
        b[i].x += b[i].vx * dt;
        b[i].y += b[i].vy * dt;
        if (b[i].x < x0 + r) {
            b[i].x = x0 + r;
            b[i].vx = -b[i].vx;
        } else if (b[i].x > x1 - r) {
            b[i].x = x1 - r;
            b[i].vx = -b[i].vx;
        }
        if (b[i].y < y0 + r) {
            b[i].y = y0 + r;
            b[i].vy = -b[i].vy;
        } else if (b[i].y > y1 - r) {
            b[i].y = y1 - r;
            b[i].vy = -b[i].vy;
        }
    }
}

void bobs_draw(const Bob *b, int n, int origin_x, int origin_y)
{
    int i, dx, dy, r2;

    for (i = 0; i < n; i++) {
        int cx = origin_x + (int)(b[i].x + 0.5f);
        int cy = origin_y + (int)(b[i].y + 0.5f);
        int r = b[i].r;
        r2 = r * r;
        for (dy = -r; dy <= r; dy++)
            for (dx = -r; dx <= r; dx++)
                if (dx * dx + dy * dy <= r2)
                    fb_put(cx + dx, cy + dy, b[i].color);
        fb_put(cx - r / 2, cy - r / 2, FB_RGB(255, 255, 255));
    }
}
