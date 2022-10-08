TARGET = procfetch
SRCS = fetch.cpp
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS = -std=c++2a -Wall --pedantic-errors
INSTALL = /usr/bin/install -c
BINDIR = /bin

all: $(TARGET)
clean:
	- rm $(TARGET) $(OBJS)
install:
	@echo "Installing $(BINDIR)/$(TARGET) ..."
	$(INSTALL) $(TARGET) $(BINDIR)/$(TARGET)
uninstall:
	- rm $(BINDIR)/$(TARGET)
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)
fetch.o: color.h

.PHONY: all clean install uninstall
