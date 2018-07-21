SUBDIRS := msi_pid \
	   msi_wordcount \
	   unit_test

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
