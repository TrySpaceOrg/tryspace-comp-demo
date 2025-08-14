# Makefile for TrySpace demonstration component CLI development
.PHONY: all build clean debug runtime start stop

export BUILDDIR ?= $(CURDIR)/build
export BUILD_IMAGE ?= tryspaceorg/tryspace-lab
export RUNTIME_NAME ?= tryspace-comp-demo-cli
export TRYLABDIR ?= $(CURDIR)/../../..
export TGTNAME ?= cpu1

# Commands
all: build

build:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name $(RUNTIME_NAME) -w $(CURDIR) $(BUILD_IMAGE) make -j build-cli

build-cli:
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && cmake .. -DTGTNAME=$(TGTNAME)
	$(MAKE) --no-print-directory -C $(BUILDDIR)

clean:
	docker run --rm -it -v $(CURDIR):$(CURDIR) --name $(RUNTIME_NAME) -w $(CURDIR) $(BUILD_IMAGE) rm -rf $(BUILDDIR)

debug:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name $(RUNTIME_NAME) -w $(CURDIR) $(BUILD_IMAGE) /bin/bash

runtime:
	$(MAKE) clean build && docker build -t $(RUNTIME_NAME) -f ../support/Dockerfile.cli .

start:
	docker run --rm -it -v $(TRYLABDIR):$(TRYLABDIR) --name "tryspace_comp_run" $(BUILD_IMAGE) ./$(BUILDDIR)/demo_cli

stop:
	docker ps --filter name=tryspace-* | xargs docker stop
