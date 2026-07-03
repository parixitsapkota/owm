#include "owm.h"

void tile(Monitor *m) {
  unsigned int i, n, h, mw, my, ty;
  Client *c;

  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
    ;
  if (n == 0) {
    return;
  }

  if (n > (unsigned int)m->nmaster) {
    mw = m->nmaster ? m->ww * m->mfact : 0;
  } else {
    mw = m->ww - m->gappx;
  }
  for (i = 0, my = ty = m->gappx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
    if (i < (unsigned int)m->nmaster) {
      h = (m->wh - my) / (MIN(n, (unsigned int)m->nmaster) - i) - m->gappx;
      resize(c, m->wx + m->gappx, m->wy + my, mw - (2 * c->bw) - m->gappx, h - (2 * c->bw), 0);
      if ((int)(my + HEIGHT(c) + m->gappx) < m->wh) {
        my += HEIGHT(c) + m->gappx;
      }
    } else {
      h = (m->wh - ty) / (n - i) - m->gappx;
      resize(c, m->wx + mw + m->gappx, m->wy + ty, m->ww - mw - (2 * c->bw) - 2 * m->gappx, h - (2 * c->bw), 0);
      if ((int)(ty + HEIGHT(c) + m->gappx) < m->wh) {
        ty += HEIGHT(c) + m->gappx;
      }
    }
  }
}

void monocle(Monitor *m) {
  unsigned int n = 0;
  Client *c;

  for (c = m->clients; c; c = c->next) {
    if (ISVISIBLE(c)) {
      n++;
    }
  }
  if (n > 0) { /* override layout symbol */
    snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
  }
  for (c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
    resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
  }
}
