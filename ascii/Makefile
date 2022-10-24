INSTALL = /usr/bin/install -c
DATADIR = /usr/share/procfetch

all: 
install:
	@echo "Installing $(DATADIR)..."
	mkdir -p $(DATADIR)/ascii
	for i in *.ascii; do             \
		$(INSTALL) "$$i" $(DATADIR)/ascii ;\
	done
uninstall:
	rm -rf $(DATADIR)

.PHONY: all check gcov clean install uninstall
