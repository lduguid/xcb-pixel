CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 $(shell pkg-config --cflags x11 xcb)
MACHINE = $(shell gcc -dumpmachine)
X11XCB = $(shell test -e /usr/lib/$(MACHINE)/libX11-xcb.so && echo -lX11-xcb || echo /usr/lib/$(MACHINE)/libX11-xcb.so.1)
LIBS = $(shell pkg-config --libs x11 xcb) $(X11XCB)

HARNESS = fb.c plat.c world.c fb.h fb_internal.h world.h
.PHONY: all clean

all: xcb-pixel xcb-pixel-bobs xcb-pixel-both

xcb-pixel: demo.c $(HARNESS)
	$(CC) $(CFLAGS) -o $@ demo.c fb.c plat.c world.c $(LIBS)

xcb-pixel-bobs: demo_bobs.c bobs.c bobs.h $(HARNESS)
	$(CC) $(CFLAGS) -o $@ demo_bobs.c bobs.c fb.c plat.c world.c $(LIBS)

xcb-pixel-both: demo_both.c bobs.c bobs.h $(HARNESS)
	$(CC) $(CFLAGS) -o $@ demo_both.c bobs.c fb.c plat.c world.c $(LIBS)

clean:
	rm -f xcb-pixel xcb-pixel-bobs xcb-pixel-both
