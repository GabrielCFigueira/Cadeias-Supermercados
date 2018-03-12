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

basename    := main
objects      = $(addsuffix .o, $(basename))

vpath %.h $(SRC_FILES):$(LIB_FILES)
vpath %.c $(SRC_FILES):$(LIB_FILES)


$(PROGRAM) : $(objects)
	$(CC) $(CFLAGS) $^ -o $@


# Source code
main.o         :

# Utils
.PHONY : clean cleaner full gp clean-p
clean   :
	$(RM) $(objects)

cleaner : clean
	$(RM) $(PROGRAM)

full    : cleaner $(PROGRAM)

