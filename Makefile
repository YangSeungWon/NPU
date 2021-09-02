CC = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = 

SRC_DIR = ./src
SRCS = $(notdir $(wildcard $(SRC_DIR)/*/*.cpp))
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)
TARGET = main.out

all: $(TARGET)

%.o: $(SRC_DIR)/*/%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ -MD $(LDFLAGS)

$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)