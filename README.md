# xcb-pixel

You are plotting **pixels** on a 320×200 playfield. There is no sprite hardware and no tilemap. The only drawing call is `fb_put`. Lines, text, and bobs are your problem — that is the point.

This is not a C64 or Amiga emulator. Fine/coarse scroll is just a small subset of that idea so demos share one pixel API.

The bitmap is VIEW + 8-pixel **margin** on each axis. The window shows a 320×200 slice at `(fine_x, fine_y)`. `fb_scroll` moves that slice one pixel at a time. After 8 pixels the bitmap is `memmove`'d by a margin and fine wrap — the same coarse/fine split as a C64 or Amiga. Newly exposed strips are **undefined**; redraw them if you keep pixels around.

Include `fb.h`. Linux and Windows. Do not include X11 or Win32.

## Build

```bash
make
./xcb-pixel
./xcb-pixel-bobs
./xcb-pixel-both
```

Windows: `make -f Makefile.win32 TAG=-mingw` (or `TOOLSET=msvc TAG=-msvc`).

## Your loop

```c
#include "fb.h"

static void tick(float dt)
{
    fb_put(10, 10, FB_RGB(255, 200, 40));
    fb_swap();
    (void)dt;
}

int main(void)
{
    return fb_main(tick);
}
```

`fb_put` writes the **back** buffer. `fb_swap` presents the visible slice and copies it to the new back buffer. Color is `0x00RRGGBB` via `FB_RGB(r,g,b)`.

Camera: `fb_cam_x` / `fb_cam_y` are the world coordinate of the visible top-left. `fb_scroll(+dx, +dy)` looks right/down and returns whether a coarse shift happened this call.

Keys: `fb_key_down(KEY_LEFT)` … `KEY_Q`. Arrows and WASD are wired in `world_scroll_keys` if you use `world.h`.

## World helper

`world.h` is a demo infinite texture, not hardware. `world_at(wx, wy)` colors a world pixel; `world_fill` paints the whole RAM window from the camera; `world_scroll_keys` pans with the arrows. Use it until you have your own map.

## Bobs

`bobs.h` is a tiny bouncing-circle helper (`bobs_init` / `bobs_update` / `bobs_draw`) so you can see objects on top of a scrolling world. Positions are bitmap-relative; pass the fine origin when drawing. Not a blitter — still `fb_put` inside.

## What to steal

| Demo | Ideas |
|------|--------|
| `demo.c` | Scroll only, crosshair at view center |
| `demo_bobs.c` | Moving circles, no scroll |
| `demo_both.c` | Scroll + bobs together |
