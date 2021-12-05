#include <string>

using namespace std;

void permutations(string in);

int main(int argc, char** argv)
{
    if (argc > 0)
    {
        permutations(argv[1]);
        return 0;
    }
    else
    {
        return -1;
    }
}
