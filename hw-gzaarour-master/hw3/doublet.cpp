
// include makefile to make doublet

/* most efficient way to build the graph: create all possible words that you can
form by replacing one letter
at a time, then looking up whether they are actually words (and if so, where
they are in your word list). */

/* A* heuristic: Incorrect Letters: counts the number of letters in the current
word which do not match the
letter in the same position in the ending word. So if you are currently at DATA
and your final word is SALT, then your heuristic will evaluate to 3. */

// make case-insensitive
/* It takes three command line parameters. The first indicates the starting
word,
the second indicates the ending word, and the third is a file which contains a
list of valid words */
// e.g. of running ./doublet head tail words.txt

/*
    make all words caps
    check all words are equal chars #

    make heap with string as item and heuristic priority as priority

*/

#include MinHeap.h

int main(int argc, char* argv[]) {}