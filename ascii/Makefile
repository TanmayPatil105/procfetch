INSTALL = /usr/bin/install -c
DATADIR = /usr/share/procfetch

all: 
install:
	@echo "Installing $(DATADIR)..."
	mkdir -p $(DATADIR)/ascii
	for i in *.ascii; do             \
		$(INSTALL) "$$i" $(DATADIR)/ascii ;\
	done

.PHONY: all clean install
