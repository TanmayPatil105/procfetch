INSTALL = /usr/bin/install -c
DATADIR = /usr/share/procfetch

all: 
install:
	echo "Installing $(DATADIR)..."
	mkdir -p $(DATADIR)
	for i in *.ascii; do             \
		$(INSTALL) "$$i" $(DATADIR) ;\
	done

.PHONY: all clean install
