#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int checkArgs(int, char*[]);
int isDir(char*);
void find(char*, char*);

int main(int argc, char* argv[]) {
  if (checkArgs(argc, argv)) {
    printf("Usage: find <dir name> <file name>\n");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}

/// @brief check whether cli-params is valid or not
/// @param argc number of args
/// @param argv args vector
/// @return 0 - valid, -1 - non-valid
int checkArgs(int argc, char* argv[]) {
  if (argc != 3 || isDir(argv[1])) {
    return -1;
  }
  return 0;
}

/// @brief check whether file in `path` is a directory or not
/// @param path file path
/// @return 0 - it is a dir, 1 - it is not a dir
int isDir(char* path) {
  int fd;
  struct stat st;
  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return 1;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return 1;
  }
  close(fd);

  // it is a dir
  if (st.type == T_DIR) {
    return 0;
  }
  return 1;
}

/**
 * @brief Recursively searches for a file with a given name in a directory and
 * its subdirectories.
 *
 * @param dirPath The path of the directory to search in.
 * @param filename The name of the file to find.
 */
void find(char* dirPath, char* filename) {
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd = open(dirPath, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", dirPath);
    return;
  }

  char buf[512], *p;
  // buf: currDir + / + subDir/file name + 0
  if (strlen(dirPath) + 1 + DIRSIZ + 1 > sizeof(buf)) {
    fprintf(2, "find: path too long\n");
    return;
  }
  strcpy(buf, dirPath);
  p = buf + strlen(buf);
  *p++ = '/';

  // check the files and walk through sub-dirs
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) {
      continue;
    }

    // skip the . and .. dir
    if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) {
      continue;
    }

    // copy full path(with file name) to buf using p
    uint deNameSize = strlen(de.name);
    memmove(p, de.name, deNameSize);
    p[deNameSize] = 0;

    // stat the subDir/file
    if (stat(buf, &st) < 0) {
      fprintf(2, "find: connot stat %s\n", buf);
      continue;
    }
    switch (st.type) {
      case T_DIR:
        find(buf, filename);
        break;

      default:
        if (!strcmp(de.name, filename)) {
          printf("%s\n", buf);
        }
        break;
    }
  }

  close(fd);
}
