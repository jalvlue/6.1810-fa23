#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: sleep <num ticks(s)>\n");
    exit(0);
  }

  int sleepTime = atoi(argv[1]);
  printf("DEBUG: sleep time: %d\n", sleepTime);
  sleep(sleepTime);
  exit(0);
}
