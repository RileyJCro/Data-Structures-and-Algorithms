#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

/* spellpath.cpp
   Riley Crockett
   May 11, 2022

   This program reads in a board, similar to one from the Neopets game "Spellseeker".
   The MaxPathLen function is dynamic, and is used to determine the length of the longest path.
   (When determining a path, adjacent nodes must have values that differ by exactly one.)
   */

/* UpdateLengths is called in MaxPathLen, and updates the max and temporary lengths */

void UpdateLengths(int &tmp_len, int &max_len)
{
  if (tmp_len > max_len) max_len = tmp_len;
  tmp_len = 1;
  return;
}

int MaxPathLen(int r, int c, vector <string> &grid)
{
  int val = grid[r][c];    // Store the initial cell value
  grid[r][c] = '-';        // "Remove" the initial cell from the grid
  int tmp_len = 1;         // Temporarily stores path length to be checked
  int max_len = 1;         // The maximum length found in the current call

  /* A vector of chars for determining available adjacent cells */

  vector <char> dirs (4, 'X');  
  if (r != 0) dirs[0] = 'U';
  if (r != (int)grid.size()-1) dirs[1] = 'D';
  if (c != 0) dirs[2] = 'L';
  if (c != (int)grid[r].size()) dirs[3] = 'R';

  if (dirs[0] == 'U' && abs(grid[r-1][c] - val) == 1) {    // Check the upwards cell
    tmp_len += MaxPathLen(r-1, c, grid);
    UpdateLengths(tmp_len, max_len);
  }
  if (dirs[1] == 'D' && abs(grid[r+1][c] - val) == 1) {    // Check the downwards cell
	tmp_len += MaxPathLen(r+1, c, grid);
    UpdateLengths(tmp_len, max_len);
  }
  if (dirs[2] == 'L' && abs(grid[r][c-1] - val) == 1) {    // Check the left cell
 	tmp_len += MaxPathLen(r, c-1, grid);
	UpdateLengths(tmp_len, max_len);
  }
  if (dirs[3] == 'R' && abs(grid[r][c+1] - val) == 1) {    // Check the right cell
	tmp_len += MaxPathLen(r, c+1, grid);
	UpdateLengths(tmp_len, max_len);
  }
  if (c % 2 == 1 && dirs[0] == 'U') {                         // Check the downwards diagonals
    if (dirs[2] == 'L' && abs(grid[r-1][c-1] - val) == 1) {
	  tmp_len += MaxPathLen(r-1, c-1, grid);
	  UpdateLengths(tmp_len, max_len);
	}
	if (dirs[3] == 'R' && abs(grid[r-1][c+1] - val) == 1) {
	  tmp_len += MaxPathLen(r-1, c+1, grid);
	  UpdateLengths(tmp_len, max_len);
	}
  } 
  if (c % 2 == 0 && dirs[1] == 'D') {                         // Check the upwards diagonals
    if (dirs[2] == 'L' && abs(grid[r+1][c-1] - val) == 1) {
	  tmp_len += MaxPathLen(r+1, c-1, grid);
	  UpdateLengths(tmp_len, max_len);
	}
	if (dirs[3] == 'R' && abs(grid[r+1][c+1] - val) == 1) {
	  tmp_len += MaxPathLen(r+1, c+1, grid);
	  UpdateLengths(tmp_len, max_len);
	}
  }
 
  grid[r][c] = val;  // Restore the initial cell's value

  return max_len;
}

int main()
{
  size_t i, j;                 // Iterators for traversing the board
  string line;                 // A string for the current line read
  vector <string> grid;        // A vector of strings for the board
  int max_len, tmp_len = 0;    // Integers for determining the max length

  while (getline(cin, line)) grid.push_back(line);    // Read in the game board

  /* Call the dynamic function on each cell of the grid and store the max length. */

  for (i = 0; i < grid.size(); i++) {
    for (j = 0; j < grid[i].size(); j++) {
      tmp_len = MaxPathLen(i, j, grid);
	  if (tmp_len > max_len) max_len = tmp_len;
	}
  }

  cout << max_len << endl;    // Print the maximum path length

  return 0;
}
