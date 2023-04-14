#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

/* matrix_enum.cpp
   Riley Crockett
   March 7, 2022

   This program enumerates a grid consisting of 'X's, 'E's, and '.'s. It is built so that
   there must be at most one 'X' in every row/column. The program takes in arguments,
   with the first two specifying grid size and the number of 'E' placements. The last
   argument specifies the printing format with 'x' displaying the grid, and 'h' displaying
   each line of the grid as an integer.
   */

class Matrix {
  public:
    int W;
    int E;
    int P;                             /* This is 'x' or 'h' */
    vector <int> Perm;                 /* Permutation of 0 .. (W-1), for the 'X' elements. */
    vector <int> Non_X;                /* This is the row/col id of each of the non-X elements. */
    vector <int> E_ID;                 /* This is the row/col id of the E elements */
    void Print();                      /* Print the matrix defined by W, Perm and E_ID */
    void Permute(int index);           /* This is the recursive permuting method. */
    void Choose(int index, size_t n);  /* This is the recursive n-choose-k method. */
};

void Matrix::Permute(int index)
{
  int i, j;
  int tmp;

  /* The base case: if the word size has been iterated.
     Store all of the Non_X entries. Call the recursive n choose k function.
	 After the n choose k returns, clear the Non_X entries and return. */

  if (index == W) {
	for (i = 0; i < W; i++) {
      for (j = 0; j < W; j++) {
        if (j != Perm[i]) { Non_X.push_back(i * W + j); }
	  }
    }
	Choose(0, E);
	Non_X.clear();
	return;
  }

  /* Loop from the current index to the word size */

  for (i = index; i < W; i++) {
    tmp = Perm[i];            // Swap the permutation at 'index' with 'i'
    Perm[i] = Perm[index];
    Perm[index] = tmp;
    Permute(index+1);         // Call the recursive function for the next index

    tmp = Perm[i];            // Undo the swap
    Perm[i] = Perm[index];
    Perm[index] = tmp;
  }
}

void Matrix::Choose(int index, size_t n)
{
  /* The 1st base case. If there are no more 'E's to place, print and return. */

  if (n == 0) {
    Print();
	return;
  }

  /* The 2nd base case. If another 'E' cannot be placed, return. */

  if (n > Non_X.size() - index) return;

  /* Push a nonX entry to the E_ID vector. Then call the recursive function
     for the next index with a decremented number of elements. */

  E_ID.push_back(Non_X[index]);
  Choose(index+1, n-1);

  /* Pop the last nonX entry from the E_ID vector. Then call the recursive function
     for the next index with the same number of elements. */

  E_ID.pop_back();
  Choose(index+1, n);
}


void Matrix::Print()
{
  size_t i;
  int num;     // An integer to store each converted line

  string s (W * W, '.');   // A W^2 string for the grid, filled with .'s

  /* Place the 'X's and 'E's at their appropriate indices in the string */
  for (int i = 0; i < W; i++) s[i * W + Perm[i]] = 'X';
  for (i = 0; i < E_ID.size(); i++) s[E_ID[i]] = 'E';

  if (P == 'x') {
	/* Print out every index of the string, with a newline after each row */
	for (i = 0; i < s.size(); i++) {
	  if (i % W == 0 && i != 0) cout << endl;
	  cout << s[i];
	}
  }
  else if (P == 'h') {
	/* For every line print it out as an integer */
	for (i = 0; i < s.size(); i++) {
	  if (i % W == 0 && i != 0) {
		cout << hex << num << endl;
		num = 0;
	  }
	  /* If the index's value is an 'X' or 'E', set the number's bit at that index */
	  if (s[i] == 'X' || s[i] == 'E') num |= (1ULL << (i % W));
	}
	cout << hex << num;
  }
  cout << endl << endl;
  return;
}

int main (int argc, char *argv[])
{
  stringstream ss;     // A stringstream to convert command line arguments
  Matrix mtx;          // An instance of the matrix class
  char mp;             // A char to help specify the print format

  if (argc > 4) return -1;

  ss << argv[1]; ss >> mtx.W;    // Store the word size
  ss.clear();
  ss << argv[2]; ss >> mtx.E;    // Store the number of 'E' placements
  ss.clear();
  ss.str("");
  ss << argv[3]; ss >> mp;
  mtx.P = mp;                    // Store the print format

  /* Initialize the first permutation indices */

  for (int i = 0; i < mtx.W; i++) mtx.Perm.push_back(i);

  mtx.Permute(0);    // Call the recursive permutation function

  return 0;
}

