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
  uint64 idx = (uint64)-1; // 使用 uint64 以匹配原始代码的返回类型
  for (int i = 0; i < elfloader.symbol_nums; i++)
  {
    if (elfloader.symbol[i].info == 18 && elfloader.symbol[i].value < ra && elfloader.symbol[i].value > boundary)
    {
      boundary = elfloader.symbol[i].value;
      idx = i; // 直接更新idx
    }
  }
  return idx; // 返回类型为 uint64，无需引入新变量
}

ssize_t sys_user_backtrace(uint64 depth) {
  uint64 u_sp = current -> trapframe -> regs.sp;
  uint64 ra = u_sp + 40;
  for(uint64 i = 0, tmp_ra = ra; i < depth; tmp_ra += 16, i++){
    uint64 index = symbol_index(*(uint64*)(tmp_ra));
    if (index != (uint64)-1) { // 确保 index 有效
      uint64 string_index = elfloader.symbol[index].name;
      sprint("%s\n", &elfloader.string[string_index]);
    }
  }
  return 0;
}

//
// [a0]: the syscall number; [a1] ... [a7]: arguments to the syscalls.
// returns the code of success, (e.g., 0 means success, fail for otherwise)
//
long do_syscall(long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7) {
  if (a0 == SYS_user_print) {
    return sys_user_print((const char*)a1, a2);
  } else if (a0 == SYS_user_exit) {
    return sys_user_exit(a1);
  } else if (a0 == SYS_user_backtrace) {
    return sys_user_backtrace(a1);
  } else {
    panic("Unknown syscall %ld \n", a0);
    return -1; // Add return here to satisfy the compiler assuming panic doesn't return
  }
}
