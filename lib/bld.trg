#!/usr/bin/bash
##  (C) by Remo Dentato (rdentato@gmail.com)
##
## This software is distributed under the terms of the MIT license:
##  https://opensource.org/licenses/MIT
##

_all () {
  bld lorem
}

_lorem () {
  bld dolor
  bld_old liblorem.a ../src/lorem.c ../src/ipsum.c && {
    cc_obj ../src/lorem.o ../src/ipsum.o
    bld_cmd $AR liblorem.a ../src/lorem.o ../src/ipsum.o ../src/dolor.o
  }
}

_dolor () {
  bld_old ../src/dolor.o ../src/dolor.c && {
    cc_obj ../src/dolor.o
  }
  bld_old libdolor.a ../src/dolor.o && {
    bld_cmd $AR libdolor.a ../src/dolor.o
  }
}

_clean () {
  bld_cmd $RM *.a
  bld_cmd $RM ../src/lorem.o ../src/ipsum.o ../src/dolor.o
}
