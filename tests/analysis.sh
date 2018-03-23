#!/bin/sh
# analysis.sh
# ASA 2018
# Gabriel Figueira, Rafael Andrade
# P1 (Sr. Joao Caracol)



set -e

ORIG_DIR=$(pwd)
cd $(dirname "$0")

EXECUTABLE="../asa.out"
N_RUNS=5
N_EXCLUDE=1
TMP_FILE=$(mktemp)
DEFAULT_CSV="../test.csv"
DEFAULT_DIR="./students-tests"
DEFAULT_GEN="./gerador"

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
    printf "scale=4; (%s0)/16 * 1000\n"  "$TIME_SUM" | bc
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


random_tests() {

  echo "Starting small random tests" >&2
  local F=`mktemp`
  for i in $(seq 0); do
    printf "Test number %d\n" "$i" >&2
    local N_SCC=`shuf -i 100-1000 -n1`
    local N_V=`shuf -i 1000-100000 -n1`
    local N_E=$((`shuf -i 2-10 -n1` * $N_V))
    local SEED=`shuf -i 0-65535 -n1`
    $DEFAULT_GEN $N_V $N_E $N_SCC 1 1000 $SEED >$F
    csv_line "$F"
  done >>"$DEFAULT_CSV"
  echo "Starting big random tests" >&2
  for i in $(seq 10); do
    printf "Test number %d\n" "$i" >&2
    local N_SCC=`shuf -i 1000-10000 -n1`
    local N_V=`shuf -i 10000-1000000 -n1`
    local N_E=$((`shuf -i 2-10 -n1` * $N_V))
    $DEFAULT_GEN $N_V $N_E $N_SCC 1 1000 $SEED >$F
    csv_line "$F"
  done >>"$DEFAULT_CSV"

}


csv_file
random_tests
cd "$ORIG_DIR"
