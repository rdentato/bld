#!/usr/bin/bash
##  (C) by Remo Dentato (rdentato@gmail.com)
##
## This software is distributed under the terms of the MIT license:
##  https://opensource.org/licenses/MIT
##

_all () {
  bld -d lib all
  bld -d test all
}

_clean () {
  bld -d lib clean
  bld -d utl clean
  bld -d test clean
}
