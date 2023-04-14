#include "fraction.hpp"
#include <cstdio>
using namespace std;

/*  fraction.cpp
	Riley Crockett
	October 19, 2021

	This program implements the Fraction class defined in "fraction.hpp".
	The fraction class manages a fraction with a numerator and denominator,
	which are two multisets that store positive integers. 
    The first 8 methods manipulate the fraction's product.
    */


/* Clear the numerator and denominator */

void Fraction::Clear()
{
  numerator.clear();
  denominator.clear();
}

/* Add a number to the numerator */

bool Fraction::Multiply_Number(int n)
{
  multiset <int>::iterator dit;      // The denominator iterator

  if (n <= 0) return false;
  if (n == 1) return true;           // No change if n is 1
  
  dit = denominator.find(n);
  
  if (dit != denominator.end()) {
    denominator.erase(dit);          // If # in the denominator, erase it 
  } else {
    numerator.insert(n);             // Otherwise insert # into the numerator
  }
  return true;
}

/* Add a number to the denominator */

bool Fraction::Divide_Number(int n)
{
  multiset <int>::iterator nit;    // The numerator iterator

  if (n <= 0) return false;
  if (n == 1) return true;         // No change if n is 1

  nit = numerator.find(n);

  if (nit != numerator.end()) {
    numerator.erase(nit);          // If # in the numerator, erase it 
  } else {
    denominator.insert(n);         // Otherwise insert # into the denominator 
  }
  return true;
}

/* Add numbers 2 through n to the numerator */

bool Fraction::Multiply_Factorial(int n)
{
  int i;                        // The loop iterator

  if (n <= 0) return false;

  for (i = 2; i <= n; i++) {
    Multiply_Number(i);
  }
  return true;
}

/* Add numbers 2 through n to the denominator */

bool Fraction::Divide_Factorial(int n)
{
  int i;                       // The loop iterator

  if (n <= 0) return false;

  for (i = 2; i <= n; i++) {
    Divide_Number(i);
  }
  return true;
}

/* Multiply n-choose-k */

bool Fraction::Multiply_Binom(int n, int k) 
{
  if (n <= 0) return false;
  if (k < 0) return false;
 
  Multiply_Factorial(n);       // Perform (n!)/(k!)(n-k)!     
  Divide_Factorial(k);
  Divide_Factorial(n-k);
  return true;
}

/* Divide n-choose-k */

bool Fraction::Divide_Binom(int n, int k)
{
  Invert();                // Swap numerator and denominator
  Multiply_Binom(n, k);    // Multiply n-choose-k
  Invert();                // Swap numerator and denominator 
  return true;
}

/* Swap the numerator and denominator */

void Fraction::Invert()
{
  numerator.swap(denominator);
}

/* Print the equation for the fraction */

void Fraction::Print() const
{
  multiset <int>::iterator nit;    // The numerator iterator
  multiset <int>::iterator dit;    // The denominator iterator

  if (numerator.empty()) {
    printf("%d", 1);               // Print '1' if numerator is empty
  } else {
	for (nit = numerator.begin(); nit != numerator.end(); nit++) {
	  if (nit != numerator.begin()) {
        printf(" * %d", *nit);     // Print numerator elements 
	  } else {
	    printf("%d", *nit);        // Print the only element in numerator 
	  }
    }
  }
  
  if (!denominator.empty()) {
    for (dit = denominator.begin(); dit != denominator.end(); dit++) {
      printf(" / %d", *dit);       // Print denominator elements if not empty
    }  
  }
  
  printf("\n");
}

/* Calculate the fraction product as a double */

double Fraction::Calculate_Product() const
{
  multiset <int>::iterator nit;    // The numerator iterator
  multiset <int>::iterator dit;    // The denominator iterator
  double prod = 1.0;

  if (!numerator.empty()) {
    for (nit = numerator.begin(); nit != numerator.end(); nit++) {
	  prod *= *nit;                // Multiply numerator elements
	}
  } 
  if (!denominator.empty()) {
    for (dit = denominator.begin(); dit != denominator.end(); dit++) {
	  prod /= *dit;                // Divide product by denominator elements
	}
  }
  return prod;
}
