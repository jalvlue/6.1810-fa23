#include "kernel/types.h"
#include "user/user.h"

int main() {
  // fd[0] for reading, fd[1] for writing
  int fd[2];
  pipe(fd);

  // buffer for holding things read from pipe
  char buf[1];

  int pid = fork();
  if (pid == 0) {
    // child
    read(fd[0], buf, 1);
    // printf("%d: DEBUG %c\n", getpid(), buf[0]);
    // printf("%d: DEBUG %s\n", getpid(), buf);
    printf("%d: received ping\n", getpid());
    write(fd[1], "C", 1);
  } else {
    // parent
    write(fd[1], "P", 1);
    read(fd[0], buf, 1);
    // printf("%d: DEBUG %c\n", getpid(), buf[0]);
    // printf("%d: DEBUG %s\n", getpid(), buf);
    printf("%d: received pong\n", getpid());
  }

  exit(0);
}
