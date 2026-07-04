#include "owm.h"

#include <X11/Xlib.h>

void movemouse(const Arg *arg) {
  (void)arg;
  int x, y, ocx, ocy, nx, ny;
  Client *c;
  Monitor *m;
  XEvent ev;
  Time lasttime = 0;

  if (!(c = selmon->sel)) {
    return;
  }
  if (c->isfullscreen) { /* no support moving fullscreen windows by mouse */
    return;
  }
  restack(selmon);
  ocx = c->x;
  ocy = c->y;
  if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync, None, cursor[CurMove]->cursor, CurrentTime) !=
      GrabSuccess) {
    return;
  }
  if (!getrootptr(&x, &y)) {
    return;
  }
  do {
    XMaskEvent(dpy, MOUSEMASK | ExposureMask | SubstructureRedirectMask, &ev);
    switch (ev.type) {
    case ConfigureRequest:
    case Expose:
    case MapRequest: handler[ev.type](&ev); break;
    case MotionNotify:
      if ((ev.xmotion.time - lasttime) <= (1000 / 60)) {
        continue;
      }
      lasttime = ev.xmotion.time;

      nx = ocx + (ev.xmotion.x - x);
      ny = ocy + (ev.xmotion.y - y);
      if (abs(selmon->wx - nx) < (int)snap) {
        nx = selmon->wx;
      } else if (abs((selmon->wx + selmon->ww) - (nx + WIDTH(c))) < (int)snap) {
        nx = selmon->wx + selmon->ww - WIDTH(c);
      }
      if (abs(selmon->wy - ny) < (int)snap) {
        ny = selmon->wy;
      } else if (abs((selmon->wy + selmon->wh) - (ny + HEIGHT(c))) < (int)snap) {
        ny = selmon->wy + selmon->wh - HEIGHT(c);
      }
      if (!c->isfloating && selmon->lt[selmon->sellt]->arrange && (abs(nx - c->x) > (int)snap || abs(ny - c->y) > (int)snap)) {
        togglefloating(NULL);
      }
      if (!selmon->lt[selmon->sellt]->arrange || c->isfloating) {
        resize(c, nx, ny, c->w, c->h, 1);
      }
      break;
    }
  } while (ev.type != ButtonRelease);
  XUngrabPointer(dpy, CurrentTime);
  if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
    sendmon(c, m);
    selmon = m;
    focus(NULL);
  }
}
