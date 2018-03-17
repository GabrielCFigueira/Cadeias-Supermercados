# Main Makefile
# ASA 2018
# Rafael Andrade, Gabriel Figueira
# P1 (Sr. Joao Caracol)

SHELL         := /bin/sh
RELEASE_NAME  := asa.out


OBJ_FILES   := build
SRC_FILES   := src
TST_FILES   := tests
TST_COMM    := ./run_tests.sh

ZIP         := zip
ZIP_FLAGS   := -ur --quiet

CLEAN_COM   := cleaner

.DEFAULT_GOAL := .DEFAULT

.DEFAULT:
	$(MAKE) $(MAKECMDGOALS) -C $(OBJ_FILES)

.PHONY : zip test do cleantest
zip :
	$(MAKE) $(CLEAN_COM)
	$(ZIP) $(ZIP_FLAGS) $(RELEASE_NAME).zip $(MAKEFILE_LIST) ||:
	$(ZIP) $(ZIP_FLAGS) $(RELEASE_NAME).zip $(SRC_FILES) ||:


test :
	cd $(TST_FILES) && $(TST_COMM)

cleantest :
	cd $(TST_FILES) && `find . -regex '.*\.gen' -delete`

do :
	$(MAKE) -C $(OBJ_FILES)
