#ifndef WORLD_H
#define WORLD_H

#include "fb.h"

Pixel world_at(int wx, int wy);
void world_fill(void);
void world_scroll_keys(float dt);

#endif
