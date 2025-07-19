# Makefile for TrySpace demonstration component development
.PHONY: all build clean debug runtime start stop

export BUILDDIR ?= $(CURDIR)/build
export BUILD_IMAGE_NAME ?= tryspace-lab
export TRYLABDIR ?= $(CURDIR)/../../..
export TGTNAME ?= cpu1

# Commands
all: build

build:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name "tryspace_comp_build" -w $(CURDIR) $(BUILD_IMAGE_NAME) make -j build-comp

build-comp:
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && cmake .. -DTGTNAME=$(TGTNAME)
	$(MAKE) --no-print-directory -C $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

debug:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name "tryspace_comp_build" -w $(CURDIR) $(BUILD_IMAGE_NAME) /bin/bash

start:
	docker run --rm -it --name "tryspace_comp_run" $(BUILD_IMAGE_NAME) ./simulith_server_standalone

stop:
	docker ps --filter name=tryspace-* --filter status=running -aq | xargs docker stop
