#include "pgmheader.h"

/*  bigwhite.cpp
    Riley Crockett
	September 6, 2021

	Given two command line arguments(rows and columns), this program
    prints a pgm file with all white pixels given these dimensions.
	*/

int main(int argc, char **argv)
{
  /* The rows, columns, error message, and stringstream */
  int rows, cols;
  string usage = "usage: bigwhite rows cols";
  istringstream ss;
	
  /* Exits if there are more than 3 arguments (including file name) */
  
  if (argc != 3) { cerr << usage << endl; return 1; }
  
  /* Clears the stringstream before attempting to read an argument */

  ss.clear();
  ss.str(argv[1]);
  
  /* Exits if the argument fails to read, or if its value is invalid */
  if (!(ss >> rows)) { cerr << usage << endl; return 1; }
  if (rows <= 0) { cerr << usage << endl; return 1; }

  ss.clear();
  ss.str(argv[2]);

  if (!(ss >> cols)) { cerr << usage << endl; return 1; }
  if (cols <= 0) { cerr << usage << endl; return 1; }
  
  /* Prints the header information */

  printHeader(rows, cols);

  /* Prints '255' for all pgm pixels */
  
  for (int i = 0; i < rows*cols; i++) printf("255\n");
  
  return 0;
}
