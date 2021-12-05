all: scheduling

scheduling: floorplan.cpp
	g++ --std=c++11 -Wall -g bst.h avlbst.h scheduling.cpp -o scheduling

.PHONY: clean
clean:
	rm -f scheduling