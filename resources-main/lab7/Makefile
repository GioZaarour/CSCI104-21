CC = g++
CPPFLAGS = -g -Wall -std=c++11
GTEST_LL = -I /usr/include/gtest/ -l gtest -l gtest_main -pthread

test: stonesTest
	./stonesTest

stonesTest: stonesTest.o stones.o
	$(CC) $(CPPFLAGS) $^ $(GTEST_LL) -o $@

stonesTest.o: stonesTest.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

stones.o: stones.cpp stones.h
	$(CC) $(CPPFLAGS) -c $< -o $@

.phony: clean test

clean:
	-@rm -rf *.o stonesTest
