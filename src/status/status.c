#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SIGPLUS SIGRTMIN
#define SIGMINUS SIGRTMIN

#define LENGTH(X) (sizeof(X) / sizeof(X[0]))
#define CMDLENGTH 50
#define MIN(a, b) ((a < b) ? a : b)
#define STATUSLENGTH (LENGTH(blocks) * CMDLENGTH + 1)

typedef struct {
  char *icon;
  char *command;
  unsigned int interval;
  unsigned int signal;
} Block;

static const Block blocks[] = {
    //         Icon    Command        Update Interval    Update Signal
    {"Mem:", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g", 30, 0},
    {"BAT:", "echo \"$(cat /sys/class/power_supply/BAT1/capacity)%\"", 30, 0},
    {"", "date '+%I:%M%p'", 30, 0},
};

static char delim[] = " | ";
static unsigned int delimLen = 3;

static char statusbar[LENGTH(blocks)][CMDLENGTH] = {0};
static char statusstr[2][STATUSLENGTH];

void getcmd(const Block *block, char *output) {
  char tempstatus[CMDLENGTH] = {0};
  strcpy(tempstatus, block->icon);
  FILE *cmdf = popen(block->command, "r");
  if (!cmdf) {
    return;
  }
  int i = strlen(block->icon);
  fgets(tempstatus + i, CMDLENGTH - i - delimLen, cmdf);
  i = strlen(tempstatus);
  if (i != 0) {
    i = tempstatus[i - 1] == '\n' ? i - 1 : i;
    if (delim[0] != '\0') {
      strncpy(tempstatus + i, delim, delimLen);
    } else {
      tempstatus[i++] = '\0';
    }
  }
  strcpy(output, tempstatus);
  pclose(cmdf);
}

void getcmds(int time) {
  const Block *current;
  for (unsigned int i = 0; i < LENGTH(blocks); i++) {
    current = blocks + i;
    if ((current->interval != 0 && time % current->interval == 0) || time == -1) {
      getcmd(current, statusbar[i]);
    }
  }
}

void getsigcmds(unsigned int signal) {
  const Block *current;
  for (unsigned int i = 0; i < LENGTH(blocks); i++) {
    current = blocks + i;
    if (current->signal == signal) {
      getcmd(current, statusbar[i]);
    }
  }
}

int getstatus(char *str, char *last) {
  strcpy(last, str);
  str[0] = '\0';
  for (unsigned int i = 0; i < LENGTH(blocks); i++) {
    strcat(str, statusbar[i]);
  }
  str[strlen(str) - strlen(delim)] = '\0';
  return strcmp(str, last); // 0 if they are the same
}

void status_init(char *dest) {
  getcmds(-1);
  getstatus(statusstr[0], statusstr[1]);
  strcpy(dest, statusstr[0]);
}

int status_update(char *dest) {
  static int timer = 0;
  getcmds(timer++);
  if (!getstatus(statusstr[0], statusstr[1])) {
    return 0;
  }
  strcpy(dest, statusstr[0]);
  return 1;
}

int status_sigupdate(int signum, char *dest) {
  getsigcmds(signum - SIGPLUS);
  if (!getstatus(statusstr[0], statusstr[1])) {
    return 0;
  }
  strcpy(dest, statusstr[0]);
  return 1;
}
