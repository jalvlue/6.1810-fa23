#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

const int MAX_BUFFER_SIZE = 100;
const int MAX_ARG_SIZE = 20;

void xargs(int, char*[], char*);

int main(int argc, char* argv[]) {
  if (argc > 1) {
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
      fprintf(2, "xargs: %s: No such file or directory\n", argv[1]);
      exit(0);
    }
    close(fd);
  }

  char buf[MAX_BUFFER_SIZE];
  gets(buf, MAX_BUFFER_SIZE);

  while (strlen(buf) > 0) {
    int pid = fork();
    if (pid == 0) {
      // child

      /* ----- debug ----- */
      // for (int i = 0; i < argc; i++) {
      // printf("DEBUG xargs arguments: %s\n", argv[i]);
      // }
      /* ----- debug ----- */

      xargs(argc, argv, buf);
    } else {
      wait(0);
      gets(buf, sizeof(buf));
    }
  }

  exit(0);
}

void xargs(int argc, char* argv[], char* buf) {
  if (argc == 1) {
    printf(buf);
    return;
  }

  // collect args
  char* execArgs[MAXARG];
  memset(execArgs, 0, sizeof(execArgs));

  // skip the argv[0], which would be "xargs"
  char** argvPtr = argv + 1;
  int argsIndex = 0;
  while (*argvPtr != 0) {
    execArgs[argsIndex] = *argvPtr;
    argsIndex += 1;
    argvPtr += 1;
  }

  // printf("DEBUG appended line: %s", buf);
  char* bufPtr = buf;
  while (*bufPtr != 0) {
    // skip the leading blanks
    for (; *bufPtr == ' '; bufPtr++)
      ;

    int bufIndex = 0;
    char tmp[MAX_ARG_SIZE];
    for (; *bufPtr != ' ' && *bufPtr != 0 && *bufPtr != '\n';
         bufPtr++, bufIndex++) {
      tmp[bufIndex] = *bufPtr;
    }
    if (*bufPtr == '\n') {
      bufPtr += 1;
    }

    if (bufIndex > 0) {
      tmp[bufIndex] = 0;
      // printf("DEBUG parse args from buf: %s\n", tmp);
      execArgs[argsIndex] = tmp;
      argsIndex += 1;
    }
  }

  /* -----debug----- */
  // printf("DEBUG: execArgs: \n");
  // p = execArgs;
  // while (*p != 0) {
  //   printf("DEBUG: %s\n", *p);
  //   p += 1;
  // }
  /* -----debug----- */

  exec(argv[1], execArgs);
}
