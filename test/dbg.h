/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
**
** This software is distributed under the terms of the MIT license:
**  https://opensource.org/licenses/MIT
**
**  *** DEBUG AND TESTING MACROS ***
**
**   dbgmsg(char *, ...)       --> Prints a message on stderr (works as printf(...)).
**                                 If DEBUG is not defined, do nothing.
** 
**   dbgtst(char *, ...)       --> Starts a test case.
**
**   dbgchk(test, char *, ...) --> Perform the test. If test fails prints a message on
**                                 stderr (works as printf(...)).
**                                 If DEBUG is not defined, do nothing.
** 
**   dbgclk( ... )             --> Measure the time needed to execute the block.
**                                 If DEBUG is not defined, execute the block but don't 
**                                 measure time.
**                                 Note how the code is enclosed by '( ... )' not '{ ... }'
** 
**   dbgblk( ... )             --> Execute the block only in DEBUG mode.
**                                 Note how the code is enclosed by '( ... )' not '{ ... }'
**
**  _dbgmsg(char *, ...)       --> Do nothing. Used to disable the debug message.
**  _dbgtst(char *, ...)       --> Do nothing. Used to disable the debug message.
**  _dbgchk(test, char *, ...) --> Do nothing. Used to disable the debug message.
**  _dbgclk( ... )             --> Execute the block but don't measure time.
**  _dbgblk( ... )             --> Do not execute the code block.
**
**  Note that NDEBUG has precedence over DEBUG 
*/

#ifndef DBG_H__
#define DBG_H__

#ifdef NDEBUG
#ifdef DEBUG
#undef DEBUG
#endif
#endif

#ifdef DEBUG
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define dbgexp(x) x
#define dbg0(x,...)   (x)

#define dbgmsg(...)   ((fflush(stdout), fprintf(stderr,__VA_ARGS__),     \
                        fprintf(stderr," \x9%s:%d\n",__FILE__,__LINE__), \
                        fflush(stderr)))

#define dbgtst(...)    dbgmsg("TEST: " __VA_ARGS__)

#define dbgchk(e,...)  do {int e_=!(e);                                                                  \
                          fflush(stdout); /* Ensure dbg message appears *after* pending stdout prints */ \
                          fprintf(stderr,"%s: (%s) \x9%s:%d\n",(e_?"FAIL":"PASS"),#e,__FILE__,__LINE__); \
                          if (e_ && *(dbgexp(dbg0(__VA_ARGS__))))                                        \
                            { fprintf(stderr,"    : " __VA_ARGS__); fputc('\n',stderr); }                \
                          fflush(stderr); errno = e_;                                                    \
                       } while(0) 

#define dbgclk(...)    do { clock_t    dbg_clk;                                    \
                            time_t     dbg_time;                                   \
                            char       dbg_tstr[32];                               \
                            struct tm *dbg_time_tm;                                \
                            long int   dbg_elapsed;                                \
                            time(&dbg_time); dbg_time_tm = localtime(&dbg_time);   \
                            strftime(dbg_tstr,32,"%Y-%m-%d %H:%M:%S",dbg_time_tm); \
                            dbg_clk = clock();                                     \
                            { __VA_ARGS__ }                                        \
                            dbg_elapsed = (long int)(clock()-dbg_clk);             \
                            dbgmsg("TIME: %s +%ld/%ld sec.",dbg_tstr,              \
                                       dbg_elapsed, (long int)CLOCKS_PER_SEC);     \
                       } while(0)

#define dbgblk(...)      do { { __VA_ARGS__ } errno = 0; } while(0)

#else
#define dbgmsg(...)
#define dbgtst(...)
#define dbgchk(e,...)
#define dbgclk(...)      do { __VA_ARGS__ } while(0)
#define dbgblk(...)
#endif

#define _dbgmsg(...)
#define _dbgtst(...)
#define _dbgchk(e,...)
#define _dbgclk(...)     do { __VA_ARGS__ } while(0)
#define _dbgblk(...)

#endif // __DBG_H__

/*<<<>>>*/
/*  ************ TESTS STATISTICS *************
**  Compile with:
**     cp dbg.h dbgstat.c; cc -DDBGSTAT -Wall -o dbgstat dbgstat.c
*/

#ifdef DBGSTAT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 1024
char s[MAXLEN];

int main(int argc, char *argv[])
{
  int n_fail = 0, n_pass = 0;
  char *p;

  while ((p=fgets(s,MAXLEN,stdin))) {
         if (strncmp(p,"PASS: ",6)==0) n_pass++; 
    else if (strncmp(p,"FAIL: ",6)==0) n_fail++; 
  }

  printf("FAIL: %d\nPASS: %d\n",n_fail,n_pass);
  exit(n_fail != 0 || n_pass == 0);
}

#endif // DBGSTAT

/*  ************ TEST SUITE *************
**  Compile with:
**     cp dbg.h dbgtest.c; cc -DDEBUG -DDBGTEST -Wall -o dbgtest dbgtest.c
*/

#ifdef DBGTEST
int main(int argc, char *argv[])
{
  int x;

  dbgmsg("Testing %s (argc: %d)","dbg library",argc);
  dbgchk(1,"");

  x=0;
  dbgtst("(1>x) with x=%d",x);
  dbgchk(1>x,"x=%d",x);

  x=1;
  dbgtst("(1>x) with x=%d",x);
  dbgchk(1>x,"x=%d",x);

  x=2;
  dbgtst("(1>x) with x=%d",x);
  dbgchk(1>x,"x=%d",x);

  dbgblk(
    int e = errno, x=0;
    if (e) {
      dbgmsg("Sigh it failed (%d)",e-x);
    }
  );

  x=3;
  dbgtst("(1>x) with x=%d (no message on fail)",x);
  dbgchk(1>x,"");

  _dbgblk(
    int e = errno, tst=0; // tst is to check that macro works
    if (e) {
      dbgmsg("Sigh it failed: (%d) but I'll never know (%d)",e,x);
    }
  );

  dbgtst("dbgclk() prints the time");
  dbgclk();

  x = 100000;
  dbgmsg("Testing count to %d",x);
  dbgclk(
    for (int k=0; k<x; k++);
  );

  x = 100000000;
  dbgmsg("Testing count to %d",x);
  dbgclk(
    for (int k=0; k<x; k++) ;
  );
}

#endif // DBG_TEST

