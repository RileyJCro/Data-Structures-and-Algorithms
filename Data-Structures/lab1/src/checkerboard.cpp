#include <iostream>
using namespace std;

/*  checkerboard.cpp
	Riley Crockett
	August 30, 2021
	
	This program reads 5 integers from standard input, which are
	then used as parameters to generate a checkerboard.

	r is the number of rows.
	c is the number of columns.
	sc is the starting character.
	cs is the cycle size.
	w is the width of the elements in the grid.
    */


int main()
{
    /* The row, column, starting char as an int, cycle size, and width */

    int r, c, isc, cs, w;

	char sc;		/* The starting char as a char */            

    /* Ensure that all 5 inputs are read */

    cin >> r >> c >> sc >> cs >> w;
		
	/* Print error message for an unsuccessful read */

	if (cin.fail()) {
		cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W" 
			 << endl;
		return -1;
	}
	
	/* Stores the starting character as an integer */

	isc = int(sc);

	/* Exits if any parameter yields a value outside of the ASCII scope */

	if ((r <= 0 || c <= 0 || sc <= 0 || cs <= 0 || w <= 0) || (sc + cs > 127)) {
		return -1;
	}
	
	/* Generate the checkerboard */

    for (int i = 0; i < r; i++) {				/* Row iterations */
		
		for (int l = 0; l < w; l++) {			/* Width applied to row */
												
			for (int j = 0; j < c; j++) {		/* Column iterations */
											
				for (int k = 0; k < w; k++) {	/* Width applied to column */

					/* Output element based on current row and column values */

					cout << char(isc + (i + j) % cs);
				}
			}

			cout << endl;
		}
	}

	return 0;
}

