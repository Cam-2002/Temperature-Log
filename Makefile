ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

default:
	mkdir -p $(ROOT_DIR)/bin
	gcc $(ROOT_DIR)/src/templog.c -o $(ROOT_DIR)/bin/templog
clean: remove default

remove:
	mkdir -p $(ROOT_DIR)/bin
	rm -r $(ROOT_DIR)/bin
