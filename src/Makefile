TARGET = procfetch
SRCS = fetch.cpp main.cpp util.cpp
OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -std=c++2a -Wall -Wextra --pedantic-errors

ifeq ($(COVERAGE_TEST), 1)
	CXXFLAGS += -fprofile-arcs -ftest-coverage -O0
	LIBS += -lgcov
endif

INSTALL = /usr/bin/install -c
FORMATTER = clang-format -i
BIN_DIR = /bin

all: $(TARGET)
run: all
	./$(TARGET)
check: all
	@if test -f not_existence; then \
		echo "Error: It is assumed that file 'not_existence' DOES NOT exist." ;\
		false ;\
	fi
	./$(TARGET) -t
gcov:
	gcov $(SRCS)
clean:
	- rm -f $(TARGET) $(OBJS) *.gcov *.gcda *.gcno
install: all
	@echo "Installing $(BIN_DIR)/$(TARGET) ..."
	$(INSTALL) $(TARGET) $(BIN_DIR)/$(TARGET)
uninstall:
	- rm $(BIN_DIR)/$(TARGET)
format:
	$(FORMATTER) $(SRCS) *.h

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)
main.o:  fetch.h color.h config.h
fetch.o: fetch.h color.h
util.o:  fetch.h color.h

.PHONY: all run check gcov clean docs install uninstall dist format gif
