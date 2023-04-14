#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

/* spellseeker.cpp
   Riley Crockett
   May 10, 2022

   This program reads in a spellseeker game board from standard input and prints it out.
   It then finds a path with the maximum length and prints it. There is a SpellSeeker 
   class with two functions used to help find the maximum path.
   */

class SpellSeeker
{
  public:
    vector <string> grid;             // The game board grid
	vector <int> path;                // Used to build a path
    vector < vector <int> > cache;    // Stores the paths with maximum length
    int max;                          // An integer for the maximum length

    void UpdateLengths(int &tmp_len, int &max_len);
    int MaxPathLen(int r, int c);
};

/* This function is called in MaxPathLen, it updates values of the max and temporary lengths. */

void SpellSeeker::UpdateLengths(int &tmp_len, int &max_len)
{
  if (tmp_len > max_len) max_len = tmp_len;
  tmp_len = 1;
  return;
}

/* This function determines the maximum path length for a cell on the grid */

int SpellSeeker::MaxPathLen(int r, int c)
{
  int row_sz = grid.size();
  int col_sz = grid[r].size();
  int tmp_len = 1, max_len = 1;    // A temporary check length and the maximum length
  int val = grid[r][c];            // Stores the initial grid value 
  grid[r][c] = '-';                // "Remove" the initial cell from the grid
  
  /* After the maximum length for the entire grid is found build paths */

  if (max != -1) path.push_back(r*col_sz + c);
  
  if (r != 0 && abs(grid[r-1][c] - val) == 1) {
    tmp_len += MaxPathLen(r-1, c);        // up in the same column
    UpdateLengths(tmp_len, max_len);
  }
  if (r != row_sz-1 && abs(grid[r+1][c] - val) == 1) {
    tmp_len += MaxPathLen(r+1, c);        // down in the same column
    UpdateLengths(tmp_len, max_len);
  }
  if (c != 0 && abs(grid[r][c-1] - val) == 1) {
    tmp_len += MaxPathLen(r, c-1);       // left in other column
    UpdateLengths(tmp_len, max_len);
  }
  if (c != col_sz && abs(grid[r][c+1] - val) == 1) {
    tmp_len += MaxPathLen(r, c+1);       // right in other column
    UpdateLengths(tmp_len, max_len);
  }
  if (c % 2 == 1 && r != 0) {     // column is odd
    if (c != 0 && abs(grid[r-1][c-1] - val) == 1) {
      tmp_len += MaxPathLen(r-1, c-1);      // r-1, c-1
      UpdateLengths(tmp_len, max_len);
    }
    if (c != col_sz && abs(grid[r-1][c+1] - val) == 1) {
      tmp_len += MaxPathLen(r-1, c+1);      // r-1, c+1
      UpdateLengths(tmp_len, max_len);
    }
  }
  if (c % 2 == 0 && r != row_sz-1) {   // column is even
    if (c != 0 && abs(grid[r+1][c-1] - val) == 1) {
      tmp_len += MaxPathLen(r+1, c-1);      // r+1, c-1
      UpdateLengths(tmp_len, max_len);
    }
    if (c != col_sz && abs(grid[r+1][c+1] - val) == 1) {
      tmp_len += MaxPathLen(r+1, c+1);      // r+1, c+1
      UpdateLengths(tmp_len, max_len);
    }
  } 

  grid[r][c] = val;  // Restore the initial cell's value

  /* If a path is built, store it in the cache if it has the maximum length,
     otherwise pop the last cell from the path. */

  if (max != -1) { 
	if ((int)path.size() == max) cache.push_back(path);
    path.pop_back();
  }

  return max_len;
}

int main()
{
  size_t i, j;
  string line;          
  SpellSeeker seek;            // An instance of the SpellSeeker class
  int max_len, tmp_len = 0;    // A temporary check length and the maximum length
  int max_r, max_c;            // Holds the start indices of a max length path
  seek.max = -1;               // Initialize the overall max length to -1

  /* Read, store, and print the grid */

  while (getline(cin, line)) { 
	cout << line << endl;
	seek.grid.push_back(line);
  }

  cout << "PATH" << endl;

  /* Loop through the grid. Find the maximum length and store that path's start indices */

  for (i = 0; i < seek.grid.size(); i++) {
    for (j = 0; j < seek.grid[i].size(); j++) {
      tmp_len = seek.MaxPathLen(i, j);
      if (tmp_len > max_len) {
        max_len = tmp_len;
        max_r = i;
        max_c = j;
      }
    }
  }

  seek.max = max_len;               // Set the overall maximum length to the maximum length
  seek.MaxPathLen(max_r, max_c);    // Now that 'max' is updated and not -1, construct the path

  int col_sz = seek.grid[0].size();

  /* Print out the path with its row and column indices */

  for (j = 0; j < seek.cache[0].size(); j++) {
    cout << seek.cache[0][j] / col_sz << " " << seek.cache[0][j] % col_sz << endl;
  }
  
  return 0;
}
