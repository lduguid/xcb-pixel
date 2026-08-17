/* Scroll-only demo. Arrows/WASD move the camera; no moving objects. */

#include "fb.h"
#include "world.h"

static void tick(float dt)
{
    int cx, cy, i;

    world_scroll_keys(dt);
    world_fill();

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
