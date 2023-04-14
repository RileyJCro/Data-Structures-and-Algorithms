#include "pgm.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

/* pgm.cpp
   Riley Crockett
   September 15, 2021

   This program implements the methods of the PGM class, which is defined
   in the pgm.hpp header file. The first three methods deal with reading,
   writing, and creating PGM files. The other methods are for formatting
   PGM files through clockwise/counter clockwise rotations, padding,
   paneling, and cropping.
   */

bool Pgm::Read(const std::string &file)
{
  ifstream fin; 
  string s;						
  size_t i, j, r, c, v;              

  fin.open(file.c_str());
  if (fin.fail()) return false;

  if (!(fin >> s)) return false;
  if (s != "P2") return false;
  if (!(fin >> c) || c <= 0) return false;
  if (!(fin >> r) || r <= 0) return false;
  if (!(fin >> i) || i != 255) return false;

  Pixels.resize(r);
  for (i = 0; i < r; i++) {
    Pixels[i].clear();
    for (j = 0; j < c; j++) {
      if (!(fin >> v) || v > 255) return false;
      Pixels[i].push_back(v);
    }
  }
  if (fin >> s) return false;
  fin.close();
  return true;
}

bool Pgm::Write(const string &file) const
{
  ofstream fout;                      // The output fstream object
  size_t i, j;                        // The row and column iterators
  size_t r, c;                        // The row and column sizes
  size_t cnt = 0;                     // A counter to limit output line size

  fout.open(file.c_str());            // Attempt to open a file for output
  if (fout.fail()) return false;

  if (Pixels.empty()) return false;   // Check for pixels to write
  r = Pixels.size();                  // Store the row and column sizes
  c = Pixels[0].size();

  /* Output the PGM header info */

  fout << "P2\n" << c << " " << r << "\n255\n";

  /* Loop through the 2d Pixel vector */

  for (i = 0; i < r; i++) {
    for (j = 0; j < c; j++) {
      fout << Pixels[i][j];               // Write a pixel at an index
      cnt++;
      if ((i + 1) * (j + 1) != r * c) {   // If it is not the last pixel
        if (!(cnt < 20)) {                // Write a newline and reset count
          fout << "\n";
          cnt = 0;
        } else {
          fout << " ";
        }
      } else {
        fout << "\n";                     // Write a newline after last pixel
      }
    }
  }
  fout.close();
  return true;
}

bool Pgm::Create(size_t r, size_t c, size_t pv)
{
  size_t i, j;         // The row and column iterators

  /* Fail on an invalid row, column, or pixel value */

  if (r == 0 || c == 0 || pv > 255) return false;

  /* Resize the 2d Pixel vector and fill it with the pixel value  */

  Pixels.resize(r);
  for (i = 0; i < r; i++) {
    Pixels[i].clear();
    for (j = 0; j < c; j++) {
      Pixels[i].push_back(pv);
    }
  }
  return true;
}

bool Pgm::Clockwise()
{
  size_t i, j;                          // The row and column iterators
  size_t pr, pc;                        // The Pixel row and column size
  vector < vector <int> > cw;           // The temporary 2d vector for rotation
  vector <int> new_col;                 // The vector for pushing back a column

  if (Pixels.empty()) return false;     // Check for pixels to write
  pc = Pixels[0].size();                // Store the row and column sizes
  pr = Pixels.size();

  /* Nest the row iterations within the column iterations.
     Push the rows into new columns so that the first row
     is now the right-most column. */

  for (i = 0; i < pc; i++) {
    for (j = 0; j < pr; j++) {
        new_col.push_back(Pixels[pr - 1 - j][i]);
    }
    cw.push_back(new_col);              // Push a column into the 2d temp vector
    new_col.clear();                    // Clear the column for new values
  }
  Pixels = cw;             // Set the 2d Pixel vector to the clockwise version
  cw.clear();              // Clear the temporary 2d vector
  return true;
}

bool Pgm::Cclockwise()
{
  /* Similar to the clockwise method but with different indexing */
  
  size_t i, j;
  size_t pr, pc;
  vector < vector <int> > ccw;
  vector <int> new_col;

  if (Pixels.empty()) return false;
  pc = Pixels[0].size();
  pr = Pixels.size();

  /* Nest the row iterations within the column iterations.
     Push the rows into new columns so that the first row
     is now the left-most column. */

  for (i = 0; i < pc; i++) {
    for (j = 0; j < pr; j++) {
        new_col.push_back(Pixels[j][pc - 1 - i]);
    }
    ccw.push_back(new_col);
    new_col.clear();
  }
  Pixels = ccw;
  ccw.clear();
  return true;
}

bool Pgm::Pad(size_t w, size_t pv)
{
  size_t i, j;                          // The new row and column iterators
  size_t r, c;                          // The new row and column size
  vector < vector <int> > padded;       // The temporary 2d vector

  if (Pixels.empty() || pv > 255) return false;     // Check for valid pixels

  r = Pixels.size() + (2 * w);          // Store the padded sizes
  c = Pixels[0].size() + (2 * w);

  padded.resize(r);

  /* Assign padded rows at the top of the image */

  for (i = 0; i < w; i++) {
    for (j = 0; j < c; j++) {
      padded[i].push_back(pv);
    }
  }

  /* Loop for the height of the original image.
     Sandwich the original image values between the padded values. */

  for (i = 0; i < Pixels.size(); i++) {
    for (j = 0; j < w; j++) {
      padded[i + w].push_back(pv);
    }
    for (j = 0; j < Pixels[i].size(); j++) {
      padded[i + w].push_back(Pixels[i][j]);
    }
    for (j = 0; j < w; j++) {
      padded[i + w].push_back(pv);
    }
  }

  /* Assign padded rows at the bottom of the image */

  for (i = 0; i < w; i++) {
    for (j = 0; j < c; j++) {
      padded[r - w + i].push_back(pv);
    }
  }
  Pixels = padded;        // Set the 2d Pixel vector to the padded version
  padded.clear();         // Clear the temporary 2d vector
  return true;
}

bool Pgm::Panel(size_t r, size_t c)
{
  size_t i, j;                           // The row and column iterators
  size_t pr, pc;                         // The Pixel vector row and column size
  vector < vector <int> > panels;        // The temporary 2d vector

  if (r == 0 || c == 0) return false;
  if (Pixels.empty()) return false;
  pr = Pixels.size();
  pc = Pixels[0].size();

  /* Resize the temporary 2d vector */

  panels.resize(r * pr);
  for (i = 0; i < panels.size(); i++) panels[i].resize(c * pc);

  /* Loop through every pixel in the paneled 2d vector.
     The paneled iterators modulo the original image's dimensions
     allows for valid indexing and copies of the same pixel values. */

  for (i = 0; i < panels.size(); i++) {
    for (j = 0; j < panels[i].size(); j++) {
      panels[i][j] = Pixels[i % pr][j % pc];
    }
  }
  Pixels = panels;
  return true;
}

bool Pgm::Crop(size_t r, size_t c, size_t rows, size_t cols)
{
  size_t i, j;                           // The row and column iterators
  size_t pr, pc;                         // The Pixel row and column size
  vector < vector <int> > cropped;       // The temporary 2d vector

  if (Pixels.empty()) return false;
  pc = Pixels[0].size();
  pr = Pixels.size();

  /* Fail if the cropping selection is not within the original dimensions */

  if (r + rows >= pr || c + cols >= pc) return false;

  /* Loop for the size of the crop selection, and
     index from the provided row and column index */

  cropped.resize(rows);
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      cropped[i].push_back(Pixels[r + i][c + j]);
    }
  }
  Pixels = cropped;
  return true;
}

