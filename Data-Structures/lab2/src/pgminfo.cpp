#include "pgmheader.h"

/*  pgminfo.cpp
    Riley Crockett
	September 6, 2021

	This program loops through a pgm file and displays the
	number of rows, columns, and pixels. It also calculates
	and displays the average pixel value.
    */

int main()
{ 
  /* The rows, columns, pixel value, total # of pixels, and pixel index */
  int rows, cols, px, num_pxs, cur_px;
  
  /* The average pixel value */
  double px_avg;

  /* The first word (P2), '255' specifier, and start of error message */
  string p2, spec, bad = "Bad PGM file -- ";
  

  /* Attempt to read the pgm header */

  if (readHeader(p2, rows, cols, spec, bad) != 0) return 1;

  /* Loop through pixels, count the # of pixels, and sum all values */
  
  num_pxs = rows*cols;
  while (cin >> px) {
    px_avg += px;
    cur_px++;
    if (cur_px > num_pxs) {
      cerr << bad << "Extra stuff after the pixels" << endl;
      return 1;
    }
    if (px < 0 || px > 255) {
      cerr << bad << "pixel " << cur_px - 1 << " is not a number between 0 and 255" << endl;
      return 1;
    }
  }

  /* Calculate the pixel average */

  if (num_pxs != 0) px_avg = px_avg / cur_px;
  
  /* Print the pgminfo */
  
  printf("# Rows: %11d\n", rows);
  printf("# Columns: %8d\n", cols);
  printf("# Pixels: %9d\n", num_pxs);
  printf("Avg Pixel: %8.3lf\n", px_avg);

  return 0;	
}
