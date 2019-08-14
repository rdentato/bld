/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
**
** This software is distributed under the terms of the MIT license:
**  https://opensource.org/licenses/MIT
*/

#include "lorem.h"

int lorem(char *buf, char *s) 
{
  sprintf(buf,"lorem(%s)",s);
  return LOREM;
}
