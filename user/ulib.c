#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

//
// wrapper so that it's OK if main() does not call exit().
//
void _main() {
  extern int main();
  main();
  exit(0);
}

char* strcpy(char* s, const char* t) {
  char* os;

  os = s;
  while ((*s++ = *t++) != 0)
    ;
  return os;
}

int strcmp(const char* p, const char* q) {
  while (*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint strlen(const char* s) {
  int n;

  for (n = 0; s[n]; n++)
    ;
  return n;
}

/**
 * @brief Sets the first 'n' bytes of the memory pointed to by 'dst' to the
 * specified value 'c'.
 *
 * This function copies the value of 'c' (converted to an unsigned char) into
 * each of the first 'n' bytes of the object pointed to by 'dst'.
 *
 * @param dst Pointer to the memory block to be filled.
 * @param c Value to be set. The value is passed as an int, but the function
 * fills the block of memory using the unsigned char conversion of this value.
 * @param n Number of bytes to be set to the value.
 * @return Pointer to the memory block 'dst'.
 */
void* memset(void* dst, int c, uint n) {
  char* cdst = (char*)dst;
  int i;
  for (i = 0; i < n; i++) {
    cdst[i] = c;
  }
  return dst;
}

char* strchr(const char* s, char c) {
  for (; *s; s++)
    if (*s == c)
      return (char*)s;
  return 0;
}

/**
 * Reads a line of input from the standard input and stores it in the specified
 * buffer.
 *
 * @param buf The buffer to store the input line.
 * @param max The maximum number of characters to read.
 * @return A pointer to the buffer containing the input line.
 */
char* gets(char* buf, int max) {
  int i, cc;
  char c;

  for (i = 0; i + 1 < max;) {
    cc = read(0, &c, 1);
    if (cc < 1)
      break;
    buf[i++] = c;
    if (c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int stat(const char* n, struct stat* st) {
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if (fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int atoi(const char* s) {
  int n;

  n = 0;
  while ('0' <= *s && *s <= '9')
    n = n * 10 + *s++ - '0';
  return n;
}

/**
 * @brief Copies a block of memory from a source address to a destination
 * address.
 *
 * This function copies the memory block specified by `vsrc` to the memory block
 * specified by `vdst`. The number of bytes to be copied is specified by `n`.
 *
 * @param vdst Pointer to the destination memory block.
 * @param vsrc Pointer to the source memory block.
 * @param n Number of bytes to be copied.
 * @return Pointer to the destination memory block (`vdst`).
 */
void* memmove(void* vdst, const void* vsrc, int n) {
  char* dst;
  const char* src;

  dst = vdst;
  src = vsrc;
  // handle possible overlap corruption when src <= dst
  if (src > dst) {
    while (n-- > 0)
      *dst++ = *src++;
  } else {
    dst += n;
    src += n;
    while (n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}

int memcmp(const void* s1, const void* s2, uint n) {
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }
  return 0;
}

void* memcpy(void* dst, const void* src, uint n) {
  return memmove(dst, src, n);
}
