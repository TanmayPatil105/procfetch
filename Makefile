TOPTARGETS := all check gcov clean install uninstall
SUBDIRS := src ascii

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ -j $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)
