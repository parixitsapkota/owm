#include "owm.h"

#include <X11/Xlib.h>

#include <locale.h>
#include <stdio.h>
#include <string.h>

Display *dpy;

int main(int argc, char *argv[]) {
  if (argc == 2 && !strcmp("-v", argv[1])) {
    die("owm-" VERSION);
  } else if (argc != 1) {
    die("usage: owm [-v]");
  }
  if (!setlocale(LC_CTYPE, "") || !XSupportsLocale()) {
    fputs("warning: no locale support\n", stderr);
  }
  if (!(dpy = XOpenDisplay(NULL))) {
    die("owm: cannot open display");
  }

  printf("[DEBUG main.c] Address of pointer variable dpy: %p\n", (void *)&dpy);
  printf("[DEBUG main.c] Value inside dpy pointer: %p\n", (void *)dpy);

  checkotherwm();
  setup();
  scan();
  run();
  cleanup();
  XCloseDisplay(dpy);
  return 1;
}
