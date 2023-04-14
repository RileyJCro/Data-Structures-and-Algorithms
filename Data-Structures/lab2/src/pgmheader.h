#include <iostream>
#include <cstdio>
#include <sstream>
#include <vector>
using namespace std;

/*  pgmheader.h
    Riley Crockett
	September 6, 2021

	This header file contains functions that are used frequently in 
	the other lab files.
    */



/* Read into header variables from cin */
/* Exits with an error message if a value does not meet pgm requirements */
   
int readHeader (string &p2, int &rows, int &cols, string &spec, string &bad) 
{
  cin >> p2 >> cols >> rows >> spec;
  if (p2 != "P2") { 
    cerr << bad << "first word is not P2"  << endl;
	return 1;
  }  
  if (cols <= 0) { 
	cerr << bad << "No column specification" << endl;
	return 1;
  }
  if (rows <= 0) {
	cerr << bad << "No row specification" << endl;
	return 1;
  }
  if (spec != "255") { 
    cerr << bad << "No 255 following the rows and columns" << endl;
	return 1;
  }
  return 0;
}

/* Read pixels from cin and store them in a 2d vector */
/* Exits with an error message if a pixel value is invalid for pgm */
int readStoreVec (vector< vector<int> > &pgm, int &px, int &cur_px, string &bad)
{
  for (unsigned i = 0; i < pgm.size(); i++) {
    for (unsigned j = 0; j < pgm[i].size(); j++) {
      if (cin >> px) {
        pgm[i][j] = px;
        cur_px++;
        if (px < 0 || px > 255) {
          cerr << bad << "pixel " << cur_px - 1 << " is not a number between 0 and 255" << endl;
		  return 1;
        }
      } 
	  else {
		cin.clear();
        cerr << bad << "pixel " << cur_px  << " is not a number between 0 and 255" << endl;
		return 1;
      }
    }
  }

  if (cin >> px) {
    cerr << bad << "Extra stuff after the pixels" << endl;
    return 1;
  }

  return 0;
}

/* Print the pgm header with formatting */
void printHeader (int &rows, int &cols)
{
  printf("P2\n%d %d\n255\n", cols, rows); 
}
