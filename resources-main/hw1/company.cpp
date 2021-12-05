#include "company.hpp"

// Initializes the tracker with n students and their 1-person companies.
CompanyTracker::CompanyTracker(int n)
{
  numCompanies = n;
  companies = new Company* [numCompanies];
  for (int i = 0; i < numCompanies; ++i)
      companies[i] = new Company ();
}

// Deallocates all dynamically allocated memory.
CompanyTracker::~CompanyTracker()
{
  // your implementation goes here
}

void CompanyTracker::merge(int i, int j)
{
  // your implementation goes here
}

void CompanyTracker::split(int i)
{
  // your implementation goes here
}

bool CompanyTracker::inSameCompany(int i, int j)
{
  // your implementation goes here
}
