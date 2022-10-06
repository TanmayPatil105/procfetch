TARGET = procfetch
SRCS = fetch.cpp
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS = -std=c++2a -Wall --pedantic-errors

all: $(TARGET)
clean:
	- rm $(TARGET) $(OBJS)
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)
fetch.o: color.h

.PHONY: all clean
