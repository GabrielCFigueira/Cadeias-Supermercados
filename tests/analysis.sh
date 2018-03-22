#!/bin/sh

set -e

EXECUTABLE="../asa.out"


memory(){
  if [ "$1" ]  && [ -f "$1" ] ; then
    valgrind "$EXECUTABLE" <"$1" 2>&1 >/dev/null | grep "allocated" | cut -d ' ' -f 11 | tr -d ','
  fi
}

run_time(){
  #Using bash more precise timing
  if [ "$1" ]  && [ -f "$1" ] ; then
    TIMEFORMAT="%U+%S"
    local TIME_SUM=$(for i in $(seq 20); do $(which time) -f "$TIMEFORMAT" "$EXECUTABLE" <"$1" 2>&1 1>/dev/null | bc; done \
                      | sort | head -n 18 | tail -n +3 | tr '\n' + )
    printf "scale=4; (%s0)/16\n"  "$TIME_SUM" | bc
  fi
}

memory "$1"
run_time "$1"
