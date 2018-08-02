SUBDIRS := src

all: src
.PHONY: all src test clean install

src:
	$(MAKE) -C src

test:
	$(MAKE) -C src test

clean:
	$(MAKE) -C src clean

install:
	$(MAKE) -C src install


