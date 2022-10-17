TARGET = procfetch
SRCS = fetch.cpp main.cpp util.cpp
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS = -std=c++2a -Wall -Wextra --pedantic-errors
INSTALL = /usr/bin/install -c
BINDIR = /bin

all: $(TARGET)
run: all
	./$(TARGET)
check: all
	./$(TARGET) -t
clean:
	- rm $(TARGET) $(OBJS)
install:
	@echo "Installing $(BINDIR)/$(TARGET) ..."
	$(INSTALL) $(TARGET) $(BINDIR)/$(TARGET)
uninstall:
	- rm $(BINDIR)/$(TARGET)
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)
main.o:  fetch.h color.h
fetch.o: fetch.h color.h
util.o:  fetch.h color.h

.PHONY: all run check clean install uninstall
