TARGET = procfetch
SRCS = main.cpp fetch.cpp
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS = -std=c++2a -Wall --pedantic-errors
INSTALL = /usr/bin/install -c
FORMATTER = clang-format -i
BINDIR = /bin

all: $(TARGET)
clean:
	- rm $(TARGET) $(OBJS)
install:
	@echo "Installing $(BINDIR)/$(TARGET) ..."
	$(INSTALL) $(TARGET) $(BINDIR)/$(TARGET)
uninstall:
	- rm $(BINDIR)/$(TARGET)
format:
	$(FORMATTER) $(SRCS) *.h

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)
main.o:  fetch.h color.h
fetch.o: fetch.h color.h

.PHONY: all clean install uninstall format
