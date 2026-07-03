#ifndef _OWM_STATUS_H
#define _OWM_STATUS_H

void status_init(char *dest);
int status_update(char *dest);
int status_sigupdate(int signum, char *dest);

#endif // _OWM_STATUS_H
