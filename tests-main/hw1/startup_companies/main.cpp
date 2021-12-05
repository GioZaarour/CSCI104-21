#include <iostream>
#include <fstream>
#include <string>

#include "company.hpp"

using namespace std;

int main(int argc, char** argv) {
    ifstream in(argv[1]);
    CompanyTracker* company_tracker;

    int n, x, y;

    string command;
    while (in >> command) {
        if (command == "START") {
            in >> n;
            company_tracker = new CompanyTracker(n);
        } else if (command == "MERGE") {
            in >> x >> y;
            company_tracker->merge(x, y);
        } else if (command == "SPLIT") {
            in >> x;
            company_tracker->split(x);
        } else if (command == "QUERY") {
            in >> x >> y;
            cout << company_tracker->inSameCompany(x, y) << " ";
        } else if (command == "QUERY_ALL") {
            for (int i = 2; i <= n - 2; ++i) {
                for (int j = 2; j <= n - 2; ++j) {
                    cout << company_tracker->inSameCompany(i, j) << " ";
                }
            }
        }
    }

    delete company_tracker;
}
