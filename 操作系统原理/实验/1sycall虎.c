/*
 * contains the implementation of all syscalls.
 */

#include <stdint.h>
#include <errno.h>

#include "util/types.h"
#include "syscall.h"
#include "string.h"
#include "process.h"
#include "util/functions.h"
#include "elf.h"

#include "spike_interface/spike_utils.h"

extern elf_ctx elfloader;
//
// implement the SYS_user_print syscall
//
ssize_t sys_user_print(const char* buf, size_t n) {
  sprint(buf);
  return 0;
}

//
// implement the SYS_user_exit syscall
//
ssize_t sys_user_exit(uint64 code) {
  sprint("User exit with code:%d.\n", code);
  // in lab1, PKE considers only one app (one process). 
  // therefore, shutdown the system when the app calls exit()
  shutdown(code);
}

uint64 symbol_index(uint64 ra)
{
  uint64 boundary = 0;
  int idx = -1;
  for (int i = 0; i < elfloader.symbol_nums; i++)
  {
    if (elfloader.symbol[i].info == 18 && elfloader.symbol[i].value < ra && elfloader.symbol[i].value > boundary)
    {
      boundary = elfloader.symbol[i].value;
      idx = i;
    }
  }
  return idx;
}

ssize_t sys_user_backtrace(uint64 depth) {
  uint64 u_sp = current -> trapframe -> regs.sp;
  uint64 ra = u_sp + 40;
  uint64 i = 0;
  for(uint64 tmp_ra = ra; i < depth; tmp_ra += 16, i ++){
    int index = symbol_index(*(uint64*)(tmp_ra));
     if (index == -1) {
        continue;
      }
      uint64 string_index = elfloader.symbol[index].name;
      sprint("%s\n", &elfloader.string[string_index]);
  }
  // sprint("%x\n",elfloader.ehdr.shoff);
  // sprint("%x\n",elfloader.ehdr.shentsize);
  // sprint("%x\n",elfloader.ehdr.shnum);
  // sprint("%x\n",elfloader.ehdr.shstrndx);
  // sprint("%d\n",elfloader.offset);
  // sprint("%s\n",elfloader.stringtable + elfloader.offset);
  return 0;
}

//
// [a0]: the syscall number; [a1] ... [a7]: arguments to the syscalls.
// returns the code of success, (e.g., 0 means success, fail for otherwise)
//
long do_syscall(long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7) {
  switch (a0) {
    case SYS_user_print:
      return sys_user_print((const char*)a1, a2);
    case SYS_user_exit:
      return sys_user_exit(a1);
    case SYS_user_backtrace:
      return sys_user_backtrace(a1);
    default:
      panic("Unknown syscall %ld \n", a0);
  }
}
