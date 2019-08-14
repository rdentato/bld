/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
**
** This software is distributed under the terms of the MIT license:
**  https://opensource.org/licenses/MIT
*/

#include "dbg.h"
#include "lorem.h"

char buf1[1024];
char buf2[1024];

int main(int argc, char *argv[])
{
  dolor(buf1,"test1");
  ipsum(buf2,buf1);
  lorem(buf1,buf2);
  dbgchk(strcmp(buf1,"lorem(ipsum(dolor(test1)))") == 0, "Got [%s]", buf1);
  return 0;
}
