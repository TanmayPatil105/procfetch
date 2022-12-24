PROJECT = procfetch
VERSION = 0.0.2
DIST_DIR = dist

TOPTARGETS := all run check gcov clean docs install uninstall dist
SUBDIRS := src ascii
.PHONY: $(TOPTARGETS) $(SUBDIRS)

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ -j $(MAKECMDGOALS)

clean:
	rm -rf docs $(DIST_DIR)

docs:
	doxygen

dist:
	@if [ ! -d $(DIST_DIR)/$(PROJECT)-$(VERSION) ]; then\
		echo "mkdir -p $(DIST_DIR)/$(PROJECT)-$(VERSION)";\
		mkdir -p $(DIST_DIR)/$(PROJECT)-$(VERSION);\
	fi
	git ls-files |\
		grep -v -f .distignore |\
		cpio -dp $(DIST_DIR)/$(PROJECT)-$(VERSION)
	tar -C$(DIST_DIR) -cf- $(PROJECT)-$(VERSION) |\
		bzip2 -cz > $(DIST_DIR)/$(PROJECT)-$(VERSION).tar.bz2
