#!/bin/sh

set -e

EXECUTABLE="../asa.out"
N_RUNS=20
N_EXCLUDE=2
TMP_FILE=$(mktemp)
DEFAULT_CSV="test.csv"
DEFAULT_DIR="./students-tests"

memory(){
  if [ "$1" ]  && [ -f "$1" ] ; then
    valgrind --tool=massif --massif-out-file="$TMP_FILE" ../asa.out <"$1"  >/dev/null 2>&1
    grep mem_heap_B "$TMP_FILE" | sed -e 's/mem_heap_B=\(.*\)/\1/' | sort -g | tail -n 1
  fi
}

run_time(){
  #Using bash more precise timing
  if [ "$1" ]  && [ -f "$1" ] ; then
    TIMEFORMAT="%U+%S"
    local TIME_SUM=$(for i in $(seq $N_RUNS); do $(which time) -f "$TIMEFORMAT" "$EXECUTABLE" <"$1" 2>&1 1>/dev/null | bc; done \
                      | sort | head -n $(($N_RUNS - $N_EXCLUDE)) | tail -n +$(($N_EXCLUDE+1)) | tr '\n' + )
    printf "scale=4; (%s0)/16\n"  "$TIME_SUM" | bc
  fi
}


csv_line(){

  if [ "$1" ] && [ -f "$1" ] ; then
    V=`head -n1 $1`
    E=`head -n2 $1 | tail -n +2`
    printf "%s,%s,%s,%.3f\n" "$V" "$E" `memory "$1"` `run_time "$1"`
  fi

}


csv_file(){
  for f in "$DEFAULT_DIR"/*.in ; do
    csv_line "$f"
  done >"$DEFAULT_CSV"
}

csv_file
