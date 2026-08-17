#ifndef FB_INTERNAL_H
#define FB_INTERNAL_H

#include "fb.h"

int fb_init(void);
void fb_shutdown(void);
Pixel *fb_back(void);
void fb_present_copy(void);
void fb_key_set(int key, int down);

#endif
