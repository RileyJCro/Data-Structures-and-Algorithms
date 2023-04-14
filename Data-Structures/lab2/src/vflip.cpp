#include "pgmheader.h"

/*  vflip.cpp
    Riley Crockett
	September 6, 2021

	This program...
	*/

int main()
{ 
  /* The rows columns, pixel value, and pixel index */
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
  for (unsigned i = 0; i < pgm.size(); i++) pgm[i].resize(cols);

  /* Attempt to read the pixels and store them in the vector */
  
  if (readStoreVec(pgm, px, cur_px, bad) != 0) return 1;

  /* Print the pgm header information */
  
  printHeader(rows, cols);
  
  /* Print the pixel values of the vertically flipped pgm */
  
  for (i = pgm.size(); i > 0; i--) {
    for (j = 0; j < pgm[i-1].size(); j++) {
      printf("%d\n", pgm[i-1][j]);
    }
  }

  return 0;
}
