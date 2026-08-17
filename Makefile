CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 $(shell pkg-config --cflags x11 xcb)
MACHINE = $(shell gcc -dumpmachine)
X11XCB = $(shell test -e /usr/lib/$(MACHINE)/libX11-xcb.so && echo -lX11-xcb || echo /usr/lib/$(MACHINE)/libX11-xcb.so.1)
LIBS = $(shell pkg-config --libs x11 xcb) $(X11XCB)

xcb-pixel: demo.c fb.c plat.c fb.h fb_internal.h
	$(CC) $(CFLAGS) -o $@ demo.c fb.c plat.c $(LIBS)

clean:
	rm -f xcb-pixel
