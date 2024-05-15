

SHIP_TARGET_DIR      = $(shell pwd)
SHIP_MAKE_FILES_FULL = $(subst Makefile,$(SHIP_TARGET_DIR)/Makefile,$(VLIB_MAKE_FILES))
SHIP_MAKEFILE_DIRS   = $(foreach FILE,$(SHIP_MAKE_FILES_FULL),$(dir $(FILE)))
SHIP_LF_DIR          = $(foreach FILE,$(VLIB_LDSCRIPT),$(dir $(realpath $(FILE))))
SHIP_LF_FNAMES       = $(foreach FILE,$(VLIB_LDSCRIPT),$(realpath $(FILE)))
SHIP_LIBS_DIRS       = $(foreach FILE,$(VLIB_LIBS),$(dir $(FILE)))

.PHONY : ship

ship:
	@echo shipping
	@echo "Making sure all objects are up to date: "
	@make
	@echo "Done"
	@echo -n "Generating shipping directory production script: "
	@echo "#!/bin/bash" > ./makeship.sh
	@echo "shopt -s extglob" >> ./makeship.sh
	@for i in $(VLIB_VPATH); do \
	  if [ -e "$$i" ]; then \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	  echo -n "cp -p $$i/!(*.a) " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	  fi \
	done
	@for i in $(VLIB_INC); do \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	  echo -n "cp -p $$i/!(*.a) " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_MAKEFILE_DIRS); do \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_MAKE_FILES_FULL); do \
	  echo -n "cp -p $$i " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_LF_DIR); do \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_LF_FNAMES); do \
	  echo -n "cp -p $$i " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_LIBS_DIRS); do \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(VLIB_LIBS); do \
	  echo -n "cp -p $$i " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/$(VLIB_DIRNAME)/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@echo "cd ./ship" >> ./makeship.sh
	@echo 'for i in $$(find ./vlib -name "*~"); do rm $$i; done' >> ./makeship.sh
	@echo 'find ./vlib -iname "*.bak" -delete' >> ./makeship.sh
	@echo "chmod -R 777 ./vlib" >> ./makeship.sh
	@echo "tar czf $$(date +%g%m%d)_$(basename $(notdir $(VLIB_APP))).tar.gz vlib" >> ./makeship.sh
	@echo "cd .." >> ./makeship.sh
	@chmod 755 ./makeship.sh
	@rm -rf ./ship
	@echo "Done"
	@echo -n "Generate shipping file: "
	@./makeship.sh
	@echo "Done"
