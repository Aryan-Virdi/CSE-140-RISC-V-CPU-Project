CC = g++

SOURCE = main.cpp
TARGET = cpu.exe
CXXFLAGS = -std=c++17 -I./include
SAMPLE1_FILE = sample/sample_part1.txt
SAMPLE2_FILE = sample/sample_part2.txt


all:
	$(CC) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

sample1: all
	./$(TARGET) $(SAMPLE1_FILE) --sample-1

sample2: all
	./$(TARGET) $(SAMPLE2_FILE) --sample-2

sample1_pl: all
	./$(TARGET) $(SAMPLE1_FILE) --sample-1 --pipelined

sample2_pl: all
	./$(TARGET) $(SAMPLE2_FILE) --sample-2 --pipelined

clean:
	rm -f $(TARGET)