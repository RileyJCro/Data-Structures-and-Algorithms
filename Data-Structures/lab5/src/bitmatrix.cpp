#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "bitmatrix.hpp"
using namespace std;

/* bitmatrix.cpp
   Riley Crockett
   October 5, 2021
   
   This program implements methods of the Bitmatrix and BM_Hash class, as well as four procedures 
   to work with bitmatrices. There are twelve Bitmatrix methods, and they can be divided into groups
   of three based on their purpose: creation, storage, access, and modification.
   The BM_Hash class and its methods support the storage and retrieval of bitmatrices.
   */

/* Create an empty bitmatrix */

Bitmatrix::Bitmatrix(int rows, int cols)
{
  int i;                                          // A loop iterator
  string empty_row;                               // A string to hold a '0' row

  if (rows <= 0) throw((string)"Bad rows");       // Exit on an invalid size
  if (cols <= 0) throw((string)"Bad cols");

  for (i = 0; i < cols; i++) empty_row += "0";    // Create one empty row

  M.clear();
  M.resize(rows, empty_row);                      // Resize the bitmatrix with empty row values
}

/* Read a bitmatrix from a file */
           
Bitmatrix::Bitmatrix(const string &fn)
{
  ifstream fin;                                        // An input fstream object
  istringstream ss;                                    // An input stringstream object
  string line;                                         // A string to hold a line of the file
  string elem;                                         // A string to hold "words" between spaces
  string row;                                          // A string to hold a line with no spaces
  size_t i;                                            // An iterator to loop through characters
  size_t col_size;                                     // A variable to store the length of a line
  size_t count = 0;                                    // A variable to check if col_size is set

  fin.open(fn.c_str());                                // Attempt to read a file
  if (fin.fail()) throw((string)"Can't open file");
     
  M.clear();

  while (getline(fin, line)) {                         // Loop through every line of the file
    if (line == "") continue;                          // Go to the next line on a blank line
	ss.clear();
	ss.str(line);                                      // Pass the line to the stringstream
	
	while (ss >> elem) {                               // Loop through every "word" using ss
      for (i = 0; i < elem.length(); i++) {            // Loop for the length of a "word"
        if ((elem[i] != '0') && (elem[i] != '1')) {   
		  throw((string)"Bad file format");            // Exit on an invalid bit value
	    }
	  }
	  row += elem;                                     // Append the element (char) to the row
	}

	if (count == 0 && row.length() != 0) {             // If the column size has not been set
      col_size = row.length();                         // Set the column size to the line length
	  count++;
	} 
	if (col_size != row.length()) {
	  throw((string)"Bad file format");                // Exit if all columns are not the same size
	}
	M.push_back(row);                                  // Assign a row to the bitmatrix
	row.clear();                                       // Clear the row string
  }
  fin.close();
}

/* Copy the current bitmatrix */

Bitmatrix *Bitmatrix::Copy() const
{
  int i, j;                              // The row and column iterators
  Bitmatrix *bp;
  
  bp = new Bitmatrix(Rows(), Cols());    // A pointer to an bitmatrix, same size as current matrix

  for (i = 0; i < Rows(); i++) {
    for (j = 0; j < Cols(); j++) {
      bp->Set(i, j, M[i][j]);            // Assign the current matrix values to the empty bitmatrix
    }
  }
  return bp;                             // Return the copy pointer
}

/* Write the bitmatrix to a file */

bool Bitmatrix::Write(const string &fn) const
{
  ofstream fout;                                          // An output fstream object
  size_t i;                                               // The vector size iterator

  if (Rows() == 0 || Cols() == 0) return false;           // Exit on empty bitmatrix
  
  fout.open(fn.c_str());                                  // Attempt to open a file for writing
  if (fout.fail()) return false;
  
  for (i = 0; i < M.size(); i++) fout << M[i] << endl;    // Write the bitmatrix to the file
  fout.close();
  return true; 
}

/* Print the bitmatrix on standard output */

void Bitmatrix::Print(size_t w) const
{                       
  int i, j;                                    // The row and column iterators
  
  if (Rows() == 0) return;                     // Exit on an empty bitmatrix
    
  for (i = 0; i < Rows(); i++) {
	if (w <= 0) {
	  printf("%s\n", M[i].c_str());            // If the width <= 0, print each row on its own line
	  continue;
	} 
	
	for (j = 0; j < Cols(); j++) {             // Otherwise loop through the number of columns
	  if (j == 0 || j % w) {                   // If on the first entry, or not on the width iteration 
		printf("%c", M[i][j]);                 // Print an entry with no preceding space
	  } 
	  else { 
		printf(" %c", M[i][j]);                // Print an entry with a preceding space
	  }
	}
	  
	if ((i + 1) % w == 0 && i < Rows()-1) {    // Add two newlines after the last row
	  printf("\n\n");
	}
	else {
	  printf("\n");                            // Add a newline between rows
	}
  }
}

/* Write the bitmatrix to a pgm file */
                 
bool Bitmatrix::PGM(const string &fn, int p, int border) const
{
  ofstream fout;                                        // An output fstream object
  int new_rows, new_cols;                               // The new pgm dimensions
  int i, j, k, l;                                       // The loop iterators

  if (Rows() == 0 || Cols() == 0) return false;         // Exit on an empty matrix
  if (p <= 0) return false;                             // Exit on an invalid pixel square value
  if (border < 0) return false;                         // Exit on a negative border value

  fout.open(fn.c_str());                                // Attempt to open a file for writing
  if (fout.fail()) return false;
  
  new_rows = (Rows() * p) + ((Rows() + 1) * border);    // Store the dimensions of the resulting pgm
  new_cols = (Cols() * p) + ((Cols() + 1) * border);

  fout << "P2\n";                                       // Write the pgm header info
  fout << new_cols << " " << new_rows << "\n255\n";
 
  if (border != 0) {                                    // If there is a border
	for (i = 0; i < border; i++) {                      // Write the top-most border rows
      for (j = 0; j < new_cols; j++) {
        fout << "0 ";
      }
      fout << endl;
    }
    fout << endl;

    for (i = 0; i < Rows(); i++) {                      // Loop for the number of matrix rows
      for (j = 0; j < p; j++) {                         // Loop for the height of a pixel square
        for (k = 0; k < Cols(); k++) {                  // Loop for the number of matrix columns
		  if (k == 0) {
            for (l = 0; l < border; l++) {              // Add a border to the start of a matrix row
			  fout << "0 ";
            }
		  }
          for (l = 0; l < p; l++) {                     // Loop for the width of a pixel square
            if (M[i][k] == '0') fout << "255 ";         // Write the pgm value associated with a bit
            if (M[i][k] == '1') fout << "100 ";
          }
          fout << endl;
          for (l = 0; l < border; l++) fout << "0 ";    // Add a border to the right of a matrix row
        }
		fout << endl;
	  }
	  for (j = 0; j < border; j++) {
	    for (k = 0; k < new_cols; k++) {                // Write border rows after every matrix row
          fout << "0 ";
        }
        fout << endl;
	  }
	}
	fout << endl;
  }   
  else {                                                // If there is no border
    for (i = 0; i < Rows(); i++) {                      // Loop for the number of matrix rows
      for (j = 0; j < p; j++) {                         // Loop for the height of a pixel square
        for (k = 0; k < Cols(); k++) {                  // Loop for the number of matrix columns
          for (l = 0; l < p; l++) {                     // Loop for the width of a pixel square
            if (M[i][k] == '0') fout << "255 ";         // Write the pgm value associated with a bit
            if (M[i][k] == '1') fout << "100 ";
          }
          fout << endl;
        }
        fout << endl;
      }
      fout << endl;
    }
  }
  fout.close();
  return true;
}

/* Return the current number of rows in the matrix */

int Bitmatrix::Rows() const
{
  return M.size();
}
                           
/* Return the current number of columns in the matrix */

int Bitmatrix::Cols() const
{
  return M[0].length();
}

/* Return the value at an index in the matrix */
                            
char Bitmatrix::Val(int row, int col) const
{
  // Check for a valid index and bitmatrix value
  if (row < 0 || row >= Rows() || col < 0 || col >= Cols()) return 'x';
  if (M[row][col] == '0' || M[row][col] == '1') return M[row][col];
  return 'x';
}
            
/* Set the value at an index in the matrix */

bool Bitmatrix::Set(int row, int col, char val)
{
  if (row < 0 || row >= Rows()) return false;    // Check for a valid row and column index
  if (col < 0 || col >= Cols()) return false;
  if (val == 0 || val == 1) {                    // Check for a valid bit value (integer)
    if (val == 0) M[row][col] = '0';             // Assign the string value at the index
    if (val == 1) M[row][col] = '1';
    return true;
  }
  else if (val == '0' || val == '1') {           // Check for a valid bit value (string)
    M[row][col] = val;                           // Assign the value at the index
    return true;
  }
  return false;
}

/* Swap the values of two rows in the matrix */
        
bool Bitmatrix::Swap_Rows(int r1, int r2)
{
  string tmp;                                           // A temporary string to hold a row's value 

  if (r1 < 0 || r2 < 0) return false;                   // Exit on any invalid row position
  if (r1 >= Rows() || r2 >= Rows()) return false;    
  
  tmp = M[r1];                                          // Set the first row's value to temp variable
  M[r1] = M[r2];                                        // Set first row to the second row
  M[r2] = tmp;                                          // Set the second row to the first row (tmp)
  return true;
}
              
/* Set a row to the sum of itself and another row */

bool Bitmatrix::R1_Plus_Equals_R2(int r1, int r2)
{
  int j;                                             // The column iterator
  int num1;                                          // An integer to store the sum
  stringstream ss;                                   // A stringstream object

  if (r1 < 0 || r2 < 0) return false;                // Exit on any invalid row position
  if (r1 >= Rows() || r2 >= Rows()) return false;
  
  for (j = 0; j < Cols(); j++) {                     // Loop for the number of matrix columns
	num1 = int(M[r1][j]) ^ int(M[r2][j]);            // Sum the individual entry values of the rows
	ss.clear();
	ss << num1;
	ss >> M[r1][j];                                  // Assign the sum to the first row's entry index
  }
  return true;
}

/* Sum two bitmatrices */

Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2)
{
  Bitmatrix *bp;                                       // A new bitmatrix pointer
  int i, j;                                            // The row and column iterators

  if (a1->Rows() != a2->Rows()) return NULL;           // Exit on matrices with different sizes
  if (a1->Cols() != a2->Cols()) return NULL;

  bp = new Bitmatrix(a1->Rows(), a2->Cols());          // Create a new bitmatrix of the same size

  for (i = 0; i < a1->Rows(); i++) {
    for (j = 0; j < a1->Cols(); j++) {
      bp->Set(i, j, a1->Val(i, j) ^ a2->Val(i, j));    // Set all entry sums to the new bitmatrix
    }
  }
  return bp;
}

/* Multiply two matrices*/

Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2)
{
  Bitmatrix *bp;                                            // A new bitmatrix pointer
  int i, j, k;                                              // The matrix iterators
  int prod = 0;                                             // An integer to store the product

  if (a1->Cols() != a2->Rows()) return NULL;                // Exit on matrices that cannot be multiplied
  
  bp = new Bitmatrix(a1->Rows(), a2->Cols());               // Create a new bitmatrix of the same size 

  for (i = 0; i < a1->Rows(); i++) {                        // Loop for the # of rows in the 1st matrix
	for (j = 0; j < a2->Cols(); j++) {                      // Loop for the # of columns in the 2nd matrix
      for (k = 0; k < a2->Rows(); k++) {                    // Loop for the # rows in the 2nd matrix
	    prod += int(a1->Val(i, k)) * int(a2->Val(k, j));    // Add entry products to the running product
	  }
	  prod %= 2;                                            // Mod the product by 2
      bp->Set(i, j, prod);                                  // Set the new bitmatrix entry to the product
	  prod = 0;                                             // Reset the product variable
	}
  }
  return bp;
}

/* Grab a subsection of bitmatrix rows */

Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const vector <int> &rows)
{
  Bitmatrix *bp;                                   // A new bitmatrix pointer
  size_t i;                                        // The row iterator
  int j;                                           // The column iterator
  size_t k;                                        // A variable that stores a row index

  for (i = 0; i < rows.size(); i++) {
    if (rows[i] < 0 || rows[i] >= a1->Rows()) {    // Exits on an invalid row index
	  return NULL;                                 
    }
  }

  bp = new Bitmatrix(rows.size(), a1->Cols());     // Create a new bitmatrix of the same size 

  for (i = 0; i < rows.size(); i++) {              // Loop through the rows of the submatrix
    k = rows[i];
    for (j = 0; j < a1->Cols(); j++) {             // Loop for the # of columns
      bp->Set(i, j, a1->Val(k, j));                // Set the new submatrix entries 
    }
  }
  return bp;
}

/* Return the inverse of a given matrix */

Bitmatrix *Inverse(const Bitmatrix *m)
{
  Bitmatrix *bp;                                 // A new bitmatrix pointer
  Bitmatrix *inv;                                // A new inverse matrix pointer
  int i, j;                                      // The row and column iterators

  if (m->Rows() != m->Cols()) return NULL;       // Exit on any non-square matrix

  bp = m->Copy();                                // Create a copy of the original matrix
  inv = new Bitmatrix(m->Rows(), m->Cols());     // Create an empty matrix for the inverse

  for (i = 0; i < inv->Rows(); i++) {            // Set 1's along the main diagonal for the inverse
    inv->Set(i, i, '1');
  }

  for (i = 0; i < bp->Rows(); i++) {             // Loop through every matrix row
    
	if (bp->Val(i, i) != '1') {                  // If there is no '1' on a main diagonal entry
	  if (i == bp->Rows()-1) return NULL;        // Exit if there is not a '1' on the bottom right
      for (j = i + 1; j < bp->Rows(); j++) {     // Loop for every row greater than i
        if (bp->Val(j, i) == '1') {              // If there is a '1' at the right diagonal entry
          bp->Swap_Rows(i, j);
          inv->Swap_Rows(i, j);                  // Swap rows i and j of the copy and inverse
		  break;
        } 
		else if (j == bp->Rows() - 1)  {         // Exit if there is no proper swap
          return NULL;
        }
      }
    }
    
	for (j = i + 1; j < bp->Rows(); j++) {       // Loop for every row greater than i
      if (bp->Val(j, i) == '1') {                // If there is a '1' below the main diagonal
        bp->R1_Plus_Equals_R2(j, i);
        inv->R1_Plus_Equals_R2(j, i);            // Replace that value by adding row i to j
      }
    }
  }

  for (i = bp->Rows()-2; i >= 0; i--) {          // Loop from the 2nd bottom-most row to the top
    for (j = i + 1; j < bp->Rows(); j++) {       // Loop for every row greater than i
      if (bp->Val(i, j) == '1') {                // If there is a '1' above the main diagonal
        bp->R1_Plus_Equals_R2(i, j);
        inv->R1_Plus_Equals_R2(i, j);            // Replace that value by adding row j to i
      }
    }
  } 
  delete bp;
  return inv;
}

/* Create a bitmatrix hash table of a given size */

BM_Hash::BM_Hash(int size)
{
  if (size <= 0) throw ((string)"Bad size");    // Exit on an invalid hash table size
  Table.resize(size);                           // Resize the hash table
}

/* Store a bitmatrix with a given key */

bool BM_Hash::Store(const string &key, Bitmatrix *bm)
{
  unsigned int h;                                    // The initial hash value 
  size_t i;                                          // An iterator variable
  size_t index;                                      // A variable to store the calculated index
  HTE hte;                                           // An instance of the hash table entry
  
  hte.key = key;                                     // Set the hte's key and bm to the given values
  hte.bm = bm;  
  
  h = 5381;                                          // Initialize the hash value

  for (i = 0; i < key.size(); i++) {                 // Loop for the length of the key
    h = (h << 5) + h + key[i];                       // Update the hash value
  }

  index = h % Table.size();                          // Set the index to the hash mod table size
  
  for (i = 0; i < Table[index].size(); i++) {
    if (Table[index][i].key == key) return false;    // Exit if the key is already in the table
  }

  Table[index].push_back(hte);                       // Store the bitmatrix at the hash table index  
  return true;
}
    
/* Retrieve a bitmatrix with the given key */

Bitmatrix *BM_Hash::Recall(const string &key) const
{
  unsigned int h;                                // The initial hash value
  size_t i;                                      // An iterator variable
  size_t index;                                  // A variable to store the calculated index
  HTE hte;                                       // An instance of the hash table entry
  Bitmatrix *bp;                                 // A new bitmatrix pointer

  h = 5381;                                      // Initialize the hash value

  for (i = 0; i < key.size(); i++) {             // Loop for the length of the key
    h = (h << 5) + h + key[i];                   // Update the hash value
  }

  index = h % Table.size();                      // Set the index to the hash mod table size

  for (i = 0; i < Table[index].size(); i++) {    // Loop through the hash table
    if (Table[index][i].key == key) {            // If the index matches a key
      hte = Table[index][i];                     // Set the hte to the hte at the hash table index 
      bp = hte.bm;                               // Set the bm pointer to point to hte's bm
      return bp;                                 // Return the bitmatrix pointer
	}
  }
  return NULL;
}
      
/* Return all of the hash table entries*/

vector <HTE> BM_Hash::All() const
{
  size_t i, j;                                 // The hash table iterators
  vector <HTE> rv;                             // A vector of hash table entries

  for (i = 0; i < Table.size(); i++) {
    for (j = 0; j < Table[i].size(); j++) {
      rv.push_back(Table[i][j]);               // Push back every hash table entry to the vector
    }
  }
  return rv;
}
