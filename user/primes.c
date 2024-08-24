#include "kernel/types.h"
#include "user/user.h"

void pipeline(int[2]);
const uint8 threashold = 32;

int main(void) {
  // fd[0] for reading, fd[1] for writing
  int fd[2];
  pipe(fd);

  int pid = fork();
  if (pid == 0) {
    // child
    close(fd[0]);
    for (uint8 i = 2; i < threashold; i++) {
      write(fd[1], &i, 1);
    }
    close(fd[1]);
  } else {
    // parent
    pipeline(fd);
  }

  exit(0);
}

void pipeline(int parentFd[2]) {
  close(parentFd[1]);

  // parent only read the first number
  // if there are numbers from left neighbor,
  // fork a child and ask the child to read and filter write numbers from left
  // while parent myself just keep pipelining
  // if not, stop pipeline, exit(0)
  /*
  +---------------------------------------------------------------------------+
  |           --> parent just capture the first number and print it,          |
  |               ch2 take over and read/filter write the following numbers   |
  |  +---+  2                                                                 |
  |  |   | --> +---+ ...     ... +---+                                        |
  |  |   | ... |   | --> ... --> |   |                                        |
  |  |ch1| --> |ch2| --> ... --> | p | parent exit last                       |
  +---------------------------------------------------------------------------+
  */
  uint8 curr;
  int n = read(parentFd[0], &curr, 1);
  if (n == 0) {
    exit(0);
  }
  printf("prime %d\n", curr);

  // fd[0] for reading, fd[1] for writing
  int fd[2];
  pipe(fd);
  uint8 buf;

  int pid = fork();
  if (pid == 0) {
    // child
    while (read(parentFd[0], &buf, 1)) {
      if (buf % curr == 0) {
        // drop
      } else {
        write(fd[1], &buf, 1);
      }
    }
    close(fd[1]);
  } else {
    // me
    pipeline(fd);
  }

  exit(0);
}
