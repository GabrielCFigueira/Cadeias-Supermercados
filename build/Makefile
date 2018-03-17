# Build Makefile
# ASA 2018
# Rafael Andrade, Gabriel Figueira
# P1

SHELL       := /bin/sh
PROGRAM     := ../asa.out

SRC_FILES   := ../src
LIB_FILES   := ../lib

CC          := gcc
CFLAGS      := -Wall -Wextra -pedantic -pedantic-errors -g
CFLAGS_R    := -O3 -ansi -Wall -lm
CFLAGS      := $(CFLAGS_R)

basename    := main
objects      = $(addsuffix .o, $(basename))

vpath %.h $(SRC_FILES):$(LIB_FILES)
vpath %.c $(SRC_FILES):$(LIB_FILES)

define compile
	$(CC) $(CFLAGS_R) $(objects) -o $(PROGRAM)
endef

.PHONY: wfastlist wlist wmatrix

wfastlist   : basename += fast_list
wfastlist   : $(objects) fast_list.o
	$(compile)

wlist   : basename += list
wlist   : $(objects) list.o
	$(compile)

wmatrix : basename += matrix
wmatrix : $(objects) matrix.o
	$(compile)


$(PROGRAM) : $(objects)


# Source code
main.o         : adj.h
list.o         : adj.h
matrix.o       : adj.h
fast_list.o    : adj.h

# Utils
.PHONY : clean cleaner full
clean   :
	$(RM) ./*.o

cleaner : clean
	$(RM) $(PROGRAM)

full    : cleaner $(PROGRAM)

