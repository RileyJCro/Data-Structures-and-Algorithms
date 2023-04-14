#include <iostream>
using namespace std;

/*  gold.cpp
	Riley Crockett
	August 25, 2021

	This program reads a map(text file) as standard input one
	character at a time, and sums the ounces of gold. The weighting of the 
	gold is based on values assigned to uppercase characters.
	*/

int main()
{
  char c;				/* The current character */
  int av = 65;		    /* The ascii value of A */
  int zv = 90;		    /* The ascii value of Z */
  int ounces = 0;		/* The total ounces of gold */
	
  /* Loop while cin can read */

  while (cin >> c) {

    /* If the char ascii value is greater than A's */ 

    if (int(c) >= av) {

      /* If the char ascii value is less than Z's */

	  if (int(c) <= zv) {
	  
	    /* Add the current ascii value - A's ascii value + 1 to ounces*/ 
	  
	    ounces += int(c) - av + 1;
	  }
    }
  }
	
  /* Print the total ounces of gold on the map */
  
  cout << ounces << endl;

  return 0;
}
