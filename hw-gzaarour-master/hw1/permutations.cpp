#include <iostream>
#include <string>

void swap(char& first, char& second);

// index start of string
int s = 0;

void permutations(std::string in) {
    int size = in.size();
    int e = size - 1;

    if (s == e) {
        std::cout << in << std::endl;
    } else {
        for (int i = s; i <= e; i++) {
            swap(in[s], in[i]);
            s += 1;

            permutations(in);

            s -= 1;  // s-1 since we incremented it above
            swap(in[s], in[i]);
        }
    }
}

void swap(char& first, char& second) {
    char temp;
    temp = first;
    first = second;
    second = temp;
}

// for testing
/*
int main (int argc, char* argv[])
{
    permutations(argv[1]);

    return 0;
} */