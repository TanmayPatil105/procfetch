TARGET = procfetch
SRCS = fetch.cpp main.cpp util.cpp
OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -std=c++2a -Wall -Wextra --pedantic-errors

ifeq ($(COVERAGE_TEST), 1)
	CXXFLAGS += -fprofile-arcs -ftest-coverage -O0
	LDFLAGS = -lgcov
endif

INSTALL = /usr/bin/install -c
BINDIR = /bin

all: $(TARGET)
check: all
	./$(TARGET) -t
gcov:
	gcov $(SRCS)
clean:
	- rm -f $(TARGET) $(OBJS) *.gcov *.gcda *.gcno
install: all
	@echo "Installing $(BINDIR)/$(TARGET) ..."
	$(INSTALL) $(TARGET) $(BINDIR)/$(TARGET)
uninstall:
	- rm $(BINDIR)/$(TARGET)
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
main.o:  fetch.h color.h
fetch.o: fetch.h color.h
util.o:  fetch.h color.h

.PHONY: all check gcov clean install uninstall
