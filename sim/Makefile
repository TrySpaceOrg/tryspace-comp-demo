# Makefile for TrySpace demonstration component SIM development
.PHONY: all build clean debug runtime start stop

export BUILDDIR ?= $(CURDIR)/build
export BUILD_IMAGE ?= tryspaceorg/tryspace-lab:0.0.0
export RUNTIME_NAME ?= tryspace-comp-demo-sim
export TRYLABDIR ?= $(CURDIR)/../../..

# Commands
all: build

build:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name $(RUNTIME_NAME) -w $(CURDIR) --user $(shell id -u):$(shell id -g) $(BUILD_IMAGE) make -j build-sim

build-sim:
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && cmake ..
	$(MAKE) --no-print-directory -C $(BUILDDIR)

clean:
	docker run --rm -it -v $(CURDIR):$(CURDIR) --name $(RUNTIME_NAME) -w $(CURDIR) $(BUILD_IMAGE) rm -rf $(BUILDDIR)

debug:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name $(RUNTIME_NAME) -w $(CURDIR) --user $(shell id -u):$(shell id -g) $(BUILD_IMAGE) /bin/bash

runtime:
	$(MAKE) clean build && docker build -t $(RUNTIME_NAME) -f ../support/Dockerfile.sim .

start:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name "tryspace_comp_run" $(BUILD_IMAGE) ./$(BUILDDIR)/demo_sim

stop:
	docker ps --filter name=tryspace-* | xargs docker stop
