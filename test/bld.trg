#!/usr/bin/bash
##  (C) by Remo Dentato (rdentato@gmail.com)
##
## This software is distributed under the terms of the MIT license:
##  https://opensource.org/licenses/MIT
##

_all () {
  bld tests
}

setflags_ () {
  CCFLAGS="$CFLAGS -I../src"
  if [[ "$DBGFLAGS" = "" ]] ; then
    DBGFLAGS="-DDEBUG=DBGLVL_TEST"
  fi
}

_tests () { # Compile each ut_xxx.c to t_xxx.exe
  local f
  local t
  setflags_
  
  bld -d ../lib lorem
  for f in $(ls ut_*.c); do
    t=${f:1}
    t=${t%.*}$_EXE
    echo "Building target: '${t%.*}'" >&2
    bld_old $t $f ../lib/liblorem.a && {
      cc_exe $t ${f%.*}.o
    }
  done
}

_default () { # Compile uxxx.c files to xxx.exe 
  local f
  local t
  setflags_
  f=u$1.c
  t=$1$_EXE
  bld -d ../lib lorem
  echo "Building target: '${t%.*}'" >&2
  bld_old $t $f ../lib/liblorem.a && {
    cc_exe $t ${f%.*}.o
  }
}

_clean () {
  bld_cmd $RM *.o
  bld_cmd $RM t_*$_EXE
  bld_cmd $RM *.log
}
