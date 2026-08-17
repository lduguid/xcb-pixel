/* Scrolling world plus bobs that bounce in view space.
 * Bob velocity is not coupled to fb_scroll: they keep their own vx/vy while
 * the playfield slides underneath. */

#include "bobs.h"
#include "fb.h"
#include "world.h"

static Bob bobs[BOB_MAX];
static int started;

static void tick(float dt)
{
    float vw = (float)fb_view_width();
    float vh = (float)fb_view_height();

    if (!started) {
        bobs_init(bobs, BOB_MAX, 0, 0, vw, vh);
        started = 1;
    }
    world_scroll_keys(dt);
    bobs_update(bobs, BOB_MAX, dt, 0, 0, vw, vh);
    world_fill();
    bobs_draw(bobs, BOB_MAX, fb_fine_x(), fb_fine_y());
    fb_swap();
}

int main(void)
{
    return fb_main(tick);
}
