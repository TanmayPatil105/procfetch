TOPTARGETS := all run check gcov clean docs install uninstall
SUBDIRS := src ascii
.PHONY: $(TOPTARGETS) $(SUBDIRS)

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ -j $(MAKECMDGOALS)

clean:
	rm -rf docs

docs:
	doxygen
