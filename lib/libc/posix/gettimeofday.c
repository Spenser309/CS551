/*
gettimeofday.c
*/

#include <sys/time.h>
#include <lib.h>

int gettimeofday(struct timeval *_RESTRICT tp, void *_RESTRICT tzp)
{
  message m;

  if (_syscall(PM_PROC_NR, GETTIMEOFDAY, &m) < 0)
  	return -1;

  tp->tv_sec = m.m2_l1;
  tp->tv_usec = m.m2_l2;

  return 0;
}

