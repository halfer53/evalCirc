//
// Created by Bruce on 6/11/2017.
//

#ifndef EVALCIRC_BPW_LINUX_H
#define EVALCIRC_BPW_LINUX_H

#ifdef __linux__
#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include <aio.h>


typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef int64_t LONGLONG;

typedef union _LARGE_INTEGER {
  struct {
    DWORD LowPart;
    LONG  HighPart;
  };
  struct {
    DWORD LowPart;
    LONG  HighPart;
  } u;
  LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;


void	err_msg(const char *, ...);			/* {App misc_source} */
void	err_dump(const char *, ...) __attribute__((noreturn));
void	err_quit(const char *, ...) __attribute__((noreturn));
void	err_cont(int, const char *, ...);
void	err_exit(int, const char *, ...) __attribute__((noreturn));
void	err_ret(const char *, ...);
void	err_sys(const char *, const char*, const char*) __attribute__((noreturn));

void	log_msg(const char *, ...);			/* {App misc_source} */
void	log_open(const char *, int, int);
void	log_quit(const char *, ...) __attribute__((noreturn));
void	log_ret(const char *, ...);
void	log_sys(const char *, ...) __attribute__((noreturn));
void	log_exit(int, const char *, ...) __attribute__((noreturn));

#define sys_chk(call) \
if ((call) == -1) { \
  err_sys("Error in `" #call "` syscall (%s:%d)", __FILE__, __LINE__); \
}

#endif


#endif //EVALCIRC_BPW_LINUX_H