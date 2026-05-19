#ifndef MY_C_UTILS_TESTS_PROCESS_HELPERS_H
#define MY_C_UTILS_TESTS_PROCESS_HELPERS_H

#include "my_c_utils/free.h"
#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>

static inline Bool run_and_expect_nonzero(void (*fn)(void))
{
  pid_t pid = fork();
  assert(pid >= 0);

  if (pid == 0)
  {
    fn();
    _exit(0);
  }

  Int status = 0;
  assert(waitpid(pid, &status, 0) == pid);
  return !WIFEXITED(status) || WEXITSTATUS(status) != 0;
}

#endif
