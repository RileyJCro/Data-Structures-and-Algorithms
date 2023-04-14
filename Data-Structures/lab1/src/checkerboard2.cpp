#include <iostream>
using namespace std;

/*	checkerboard.cpp
	Riley Crockett
	August 30, 2021


	*/

int main()
{
	int r, c, isc, cs, w;
	char sc;
	int nc = 0; // num characters

	// loops while cin does not fail and the number of chars is <= 5
	while ((!cin.fail()) && (nc <= 5)) {

		nc++;

		switch (nc) {
			case 1:
				cin >> r;
				break;
			case 2:
				cin >> c;
				break;
			case 3:
				cin >> sc;
				isc = int(sc);
				break;
			case 4:
				cin >> cs;
				break;
			case 5:
				cin >> w;
				break;
		}

		if (cin.fail()) {
			cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W" << endl;
			return -1;
		}
	}

	// silently exits if any input is less than zero or if start char + cycle size > 127
	if (((r <= 0) || (c <= 0) || (sc <= 0) || (cs <= 0) || (w <= 0)) || (sc + cs > 127)) {
		return 0;
	}
	
	// row iteration
    for (int i = 0; i < r; i++) {
		
		for (int l = 0; l < w; l++) {

        // column iteration check
        for (int j = 0; j < c; j++) {
			
			// width iteration check
			for (int k = 0; k < w; k++) {
				cout << char(isc + (i + j) % cs);
			}
		}

		cout << endl;
	}
	}

	return 0;
}

