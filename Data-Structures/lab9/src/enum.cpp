#include <iostream>
#include <sstream>
using namespace std;

/* enum.cpp
   Riley Crockett
   November 9, 2021

   This program prints all combinations of a string, containing only 
   0's and 1's with a specified length and number of ones, in sorted order. 
   This is done through recursive calls to the "do_enumeration" function.
   */

void do_enumeration(string &s, int index, int n_ones) 
{
  int s_len = s.size();      // An integer to store the string length 

  /* Print the string and return after traversing its length (base case) */

  if (index == s_len) { cout << s << endl; return; }

  /* Set the value at the string index to '0', and call the function 
     again for the next index with the same number of ones. */

  if (index + n_ones < s_len) {
    s[index] = '0';
    do_enumeration(s, index + 1, n_ones);
  }

  /* Set the value at the string index to '1', and call the function
     again for the next index with 1 less number of ones. */

  if (n_ones > 0) {
    s[index] = '1';
    do_enumeration(s, index + 1, n_ones - 1);
  }
}

int main(int argc, char *argv[])
{
  string s = "";         // A string to store permutations
  istringstream ss;      // A sstream to parse cmd line arguments
  int length, n_ones;    // Integers for string length and number of 1's
  int index = 0;         // An integer for the index of the string

  if (argc != 3) return 1;

  ss.str(argv[1]); ss >> length; 
  ss.clear();
  ss.str(argv[2]); ss >> n_ones;

  /* Create a string with "length" fill characters */  
  
  for (int i = 0; i < length; ++i) { s += "-"; }
  
  do_enumeration(s, index, n_ones);  
  
  return 0;
}
