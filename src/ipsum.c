/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
**
** This software is distributed under the terms of the MIT license:
**  https://opensource.org/licenses/MIT
*/

#include "lorem.h"

int ipsum(char *buf, char *s) 
{
  sprintf(buf,"ipsum(%s)",s);
  return IPSUM;
}
