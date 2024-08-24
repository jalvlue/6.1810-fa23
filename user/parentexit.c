#include "kernel/types.h"
#include "user/user.h"

// test whether shell prompts would pop up after parent exit
int main() {
  int pid = fork();
  if (pid == 0) {
    // child
    sleep(10);
    for (int i = 0; i < 5; i++) {
      printf("%d: child say %d\n", getpid(), i);
    }
  } else {
    // parent just exit directly
    printf("%d: parent exit directly\n", getpid());
  }

  exit(0);
}
