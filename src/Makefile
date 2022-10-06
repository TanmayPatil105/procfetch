TARGET = procfetch
SRCS = fetch.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)
clean:
	- rm $(TARGET)	
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)
fetch.o: color.h

.PHONY: all clean
