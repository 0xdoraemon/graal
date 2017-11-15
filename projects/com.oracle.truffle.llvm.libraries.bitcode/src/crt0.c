/*
 * Copyright (c) 2017, Oracle and/or its affiliates.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __linux__
#include <elf.h>
#else
#define AT_NULL 0
typedef struct {
  long a_type;
  union {
    long a_val;
  } a_un;
} Elf64_auxv_t;
#endif

extern char **environ;

int main(int argc, char **argv, char **envp);

static Elf64_auxv_t *__auxv;

__attribute__((weak)) int _start(long *p, int type) {
  int argc = p[0];
  char **argv = (void *)(p + 1);
  char **envp = argv + argc + 1;

  int envc = 0;
  char **ptr;
  for (ptr = envp; *ptr; ptr++) {
    envc++;
  }

  environ = envp;
  __auxv = (Elf64_auxv_t *)(envp + envc + 1);

  switch (type) {
  /* C/C++/... */
  default:
  case 0:
    exit(main(argc, argv, envp));
    break;
  /* Rust */
  case 1: {
    long (*i64main)(long argc, char **argv) = (long (*)(long, char **))main;
    exit(i64main(argc, argv));
    break;
  }
  /* non-standard C: void main(int, char**, char**) */
  case 2: {
    void (*vmain)(int argc, char **argv, char **envp) = (void (*)(int, char **, char **))main;
    vmain(argc, argv, envp);
    exit(0);
    break;
  }
  /* non-standard C: char main(int, char**, char**) */
  case 3: {
    char (*i8main)(int argc, char **argv, char **envp) = (char (*)(int, char **, char **))main;
    exit(i8main(argc, argv, envp));
    break;
  }
  /* non-standard C: short main(int, char**, char**) */
  case 4: {
    short (*i16main)(int argc, char **argv, char **envp) = (short (*)(int, char **, char **))main;
    exit(i16main(argc, argv, envp));
    break;
  }
  /* non-standard C: long main(int, char**, char**) */
  case 5: {
    long (*i64main)(int argc, char **argv, char **envp) = (long (*)(int, char **, char **))main;
    exit(i64main(argc, argv, envp));
    break;
  }
  }
  abort();
}

#ifdef __linux__
__attribute__((weak)) unsigned long getauxval(unsigned long type) {
  Elf64_auxv_t *auxv;
  for (auxv = __auxv; auxv->a_type != AT_NULL; auxv++) {
    if (auxv->a_type == type) {
      return auxv->a_un.a_val;
    }
  }
  return 0;
}
#endif
