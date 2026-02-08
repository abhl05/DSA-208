CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = hash_table
SRC = hash_table.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
