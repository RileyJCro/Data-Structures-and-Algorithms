#include "pgmheader.h"

/*  neg.cpp
    Riley Crockett
	September 6, 2021

	This program reads a pgm file from standard input, and prints
	the negative of it.
	*/

int main()
{ 
  /* The rows, columns, pixel value, and pixel index */
  int rows, cols, px, cur_px;
  
  /* The iterators for the rows and columns respectively */
  unsigned i, j;
  
  /* The first word, '255' specifier, and start of error message */
  string p2, spec, bad = "Bad PGM file -- ";
  

  /* Initialize a 2d vector to store pixels */

  vector < vector <int> > pgm;
  
  /* Attempt to read the pgm header */

  if (readHeader(p2, rows, cols, spec, bad) != 1) {
    pgm.resize(rows);
    for (i = 0; i < pgm.size(); i++) pgm[i].resize(cols);

    /* Read in the pixels and store them in the vector.
       Exit if there is an invalid pixel value. */

    if (readStoreVec(pgm, px, cur_px, bad) != 1) {

	  /* Print the pgm header information */

      printHeader(rows, cols);

      /* Print the negative of all pixels */

      for (i = 0; i < pgm.size(); i++) {
        for (j = 0; j < pgm[i].size(); j++) {
          printf("%d\n", (255 - pgm[i][j]));
        }
      } 
	} 
	else {
	  return 1; 
	}
  } 
  else {
    return 1;
  }

  return 0;
}
