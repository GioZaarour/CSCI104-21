#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include "bst.h"
#include "avlbst.h"

struct student {
    std::string name;
    std::vector<int> classes;
};

AVLTree<int, int> avl;
//vector to track slots each student used up so far (in backtracking algo)
std::vector<int> slotsUsed;
//vector to hold all the students
std::vector<student> stu;

void search (student s, int numStudents, int numSlots, int index) {

    slotsUsed.clear();

    //for all classes the student is in
    for(int i = 0; i < s.classes.size(); i++){
        //if the current slot assigned to the class node in the AVL tree is unused (for this student)
        if( std::find(slotsUsed.begin(), slotsUsed.end(), avl.find(s.classes[i])->second ) == slotsUsed.end() ) {
            //add it to slotsUsed
            slotsUsed.push_back( avl.find(s.classes[i])->second );
        }
        else {
            //increment that slot (and wrap around so we don't exceed the number of slots we have)
            if (avl.find( s.classes[i] )->second == numSlots) {
                //make sure it wraps around to slot 1 if we're at the last slot. with %
                avl.find( s.classes[i] )->second = ( avl.find( s.classes[i] )->second + 1 ) % numSlots;

            }
            avl.find( s.classes[i] )->second = avl.find( s.classes[i] )->second + 1 ;
        }
    }

    //done for every single class that student is taking, so now call search on the next student
    if(index < numStudents) {
        search(stu[index+1], numStudents, numSlots, index+1);
    }
    else { //done with all students
        return;
    }
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Please include an input file argument" << std::endl;
        return 0;
    }
    std::ifstream input(argv[1]);
    if (input.fail()) {
        std::cout << "Not a valid file" << std::endl;
        return 0;
    }

    int numClasses, numStudents, numSlots;
    input >> numClasses >> numStudents >> numSlots;

    //variables to parse the input file how we want (see loop below)
    std::string inputLine;
    std::stringstream stringStream(inputLine);
    std::string inputWords;

    //use one student object to push to the vector
    //the loop below will re-assign values every time which is fine since push_back is pass by value
    student addStudent;

    //vector to hold a list of all existing classes
    std::vector<int> allClasses;

    for (int i = 0; i < numStudents; i++) {

        //clear out the addStudent object
        addStudent.name = "";
        addStudent.classes.clear();

        inputLine = "";
        getline(input, inputLine);

        stringStream >> addStudent.name;

        while (stringStream >> inputWords) {
            
            //if the class isn't already in the allClasses vector, add it
            if( std::find( allClasses.begin(), allClasses.end(), stoi(inputWords) ) == allClasses.end() ){
                allClasses.push_back( stoi(inputWords) );
            }

            addStudent.classes.push_back( stoi(inputWords) );
            inputWords = "";
        }

        //now addStudent is finished
        stu.push_back(addStudent);
    }

    //construct AVL tree with all the class numbers (key) and assign them random time slots (value)
    std::pair<int, int> addNode;
    srand (time(NULL));
    for (int i = 0; i < numClasses; i++) {
        addNode.first = allClasses[i];
        addNode.second = rand() % numSlots + 1;

        avl.insert(addNode);
    }
    
    int index =0;
    search(stu[0], numStudents, numSlots, index);

    //print out the whole AVL tree. "key value" "class timeslot"
    for (int i = 0; i < allClasses.size(); i++ ) {
            std::cout << allClasses[i] << " ";
            std::cout << avl.find( allClasses[i] )->second << std::endl;
    }

    return 0;
}
