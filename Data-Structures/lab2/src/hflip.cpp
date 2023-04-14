#include "pgmheader.h"

/*  hflip.cpp
    Riley Crockett
	September 6, 2021
    
	This program reads a pgm file and prints it back but 
	with a horizontal flip.
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
  
  /* Attempt to read the pgm file header */

  if (readHeader(p2, rows, cols, spec, bad) != 0) return 1;
  
  /* Resize the 2d vector with the number of rows and columns */
  
  pgm.resize(rows);
  for (i = 0; i < pgm.size(); i++) pgm[i].resize(cols);

  /* Attempt to read the pixels and store them in the vector */
  
  if (readStoreVec(pgm, px, cur_px, bad) != 0) return 1;
  
  /* Print the pgm header information */
  
  printHeader(rows, cols);

  /* Print the pixel values of the horizontally flipped pgm */
  
  for (i = 0; i < pgm.size(); i++) {
    for (j = pgm[i].size(); j > 0; j--) {
      printf("%d\n", pgm[i][j-1]);
    }
  }

  return 0;
}

