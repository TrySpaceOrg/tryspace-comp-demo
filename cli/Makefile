# Makefile for TrySpace demonstration component CLI development
.PHONY: all build clean debug runtime start stop

export BUILDDIR ?= $(CURDIR)/build
export BUILD_IMAGE_NAME ?= tryspace-lab
export RUNTIME_NAME ?= tryspace-comp-demo-cli
export TRYLABDIR ?= $(CURDIR)/../../..
export TGTNAME ?= cpu1

# Commands
all: build

build:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name "tryspace_comp_build" -w $(CURDIR) $(BUILD_IMAGE_NAME) make -j build-cli

build-cli:
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && cmake .. -DTGTNAME=$(TGTNAME)
	$(MAKE) --no-print-directory -C $(BUILDDIR)

clean:
	docker run --rm -it -v $(CURDIR):$(CURDIR) --name "tryspace_comp_build" -w $(CURDIR) $(BUILD_IMAGE_NAME) rm -rf $(BUILDDIR)

debug:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name "tryspace_comp_build" -w $(CURDIR) $(BUILD_IMAGE_NAME) /bin/bash

runtime:
	$(MAKE) clean build && docker build -t $(RUNTIME_NAME) -f ../support/Dockerfile.runtime .

start:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name "tryspace_comp_run" $(BUILD_IMAGE_NAME) ./$(BUILDDIR)/demo_cli

stop:
	docker ps --filter name=tryspace-* --filter status=running -aq | xargs docker stop
