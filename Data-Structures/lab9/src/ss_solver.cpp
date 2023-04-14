#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

/* ss_solver.cpp
   Riley Crockett
   November 9, 2021
   
   This program solves a puzzle consisting of a grid (0's and 1's) and a series 
   of shapes. The goal is to have the grid filled with 1's, by applying shapes that
   flip the grid's values. It tests where a shape can be placed within the grid, and 
   combinations of those placements to try and determine the solution. 
   
   The Solver class defines the methods and variables used to do this. 
   main() is at the bottom of the program and uses an instance of the Solver class.
   */

class Solver {
  public:
    
	/* Reads the grid from cmd line arguments, shapes from standard input,
	   and creates another grid filled with 1's */
	
	bool Read_Grid_And_Shapes(int argc, const char *argv[]);
    
	/* If a shape can be placed in the grid at the row and column indices,
	   it applies the shape to the grid, otherwise it returns false */

    bool Place_Shape(size_t index, size_t r, size_t c);
    
	/* Finds all valid positions of a shape within the grid and stores 
	   the row and column indices in valid_rows and valid_columns */

    void All_Shape_Positions(size_t index);

	/* A recursive method that places valid combinations of shapes 
	   in the grid to try and find the solution */

    bool Find_Solution(size_t index);

    void Print_Solution() const;    // Prints the solution if found

    size_t Num_Shapes();    // Returns the number of shapes

  protected:

	std::vector <std::string> Grid; 
    std::vector <std::string> Full_Grid;

    std::vector < std::vector <std::string> > Shapes;
    
	std::vector < std::vector <int> > Valid_Rows;
    std::vector < std::vector <int> > Valid_Cols;
    
	std::vector <int> Solution_Rows;
    std::vector <int> Solution_Cols;
};

bool Solver::Read_Grid_And_Shapes(int argc, const char *argv[])
{
  string line, shape_comp;    // Strings for an input line, and shape component
  istringstream ss;           // A sstream for parsing input
  vector <string> shape;      // A vector of strings to store a shape's components
  string ones;                // A string used to fill a grid with all 1's
  size_t i;                   // An iterator

  /* Read the grid from cmd line arguments */

  for (int i = 1; i < argc; ++i) { Grid.push_back(argv[i]); }

  while (getline(cin, line)) {
    ss.clear(); 
	ss.str(line);
	
	/* Fill the shape vector with each word/component */
	
	while (ss >> shape_comp) { shape.push_back(shape_comp); }

	/* Store the shape in shapes, then clear the shape value */

	Shapes.push_back(shape);
	shape.clear();
  }

  /* Fill Full_Grid with 1's */
  
  for (i = 0; i < Grid[0].size(); ++i) { ones += "1"; }
  for (i = 0; i < Grid.size(); ++i) { Full_Grid.push_back(ones); }
  
  return true;
}

bool Solver::Place_Shape(size_t index, size_t r, size_t c)
{
  size_t i, j;

  /* Check if the row index and shape height fit the grid */

  if (Grid.size() < r + Shapes[index].size()) return false;

  for (i = 0; i < Shapes[index].size(); ++i) {
    
	/* Check if the column index and shape width fit the grid */

	if (Grid[r].size() < c + Shapes[index][i].size()) return false;

	for (j = 0; j < Shapes[index][i].size(); ++j) {
	  
	  /* Flip the grid value if the shape has a '1' at the current position */

	  if (Shapes[index][i][j] == '1') {
        Grid[r + i][c + j] = (Grid[r + i][c + j] == '1') ? '0' : '1';
      }
    }
  }
  return true;
}

void Solver::All_Shape_Positions(size_t index)
{
  size_t i, j;
  vector <int> rows, cols;

  for (i = 0; i < Grid.size(); ++i) {
    for (j = 0; j < Grid[i].size(); ++j) {	  

	  /* Store the grid indices if the shape can be placed, and undo the shape */
	  
	  if (Place_Shape(index, i, j)) {
        rows.push_back(i); 
		cols.push_back(j);
        Place_Shape(index, i, j);
      }
    }
  }
  Valid_Rows.push_back(rows); 
  Valid_Cols.push_back(cols);
}

bool Solver::Find_Solution(size_t index)
{
  size_t i;
  
  /* Base case */

  if (index == Shapes.size()) {
	if (Grid == Full_Grid) return true;
	return false;
  }

  for (i = 0; i < Valid_Rows[index].size(); ++i) {
    
	/* Place the indexed shape and push valid grid indices to the solutions */

    Place_Shape(index, Valid_Rows[index][i], Valid_Cols[index][i]);
    Solution_Rows.push_back(Valid_Rows[index][i]);
    Solution_Cols.push_back(Valid_Cols[index][i]);
    
    /* Call the function again for the next shape, return true if a solution was found */

    if (Find_Solution(index + 1)) return true; 
    
	/* Otherwise pop the last grid indices from the solutions and undo the shape */
    
	Solution_Rows.pop_back(); 
	Solution_Cols.pop_back();
	Place_Shape(index, Valid_Rows[index][i], Valid_Cols[index][i]);
  }
  return false;
}

void Solver::Print_Solution() const
{
  size_t i, j;
  
  if (Solution_Rows.empty()) return;

  /* Print every shape with its components and the correct grid indices */

  for (i = 0; i < Shapes.size(); ++i) {
	for (j = 0; j < Shapes[i].size(); ++j) { cout << Shapes[i][j] << " "; }	
	cout << Solution_Rows[i] << " " << Solution_Cols[i] << endl;
  }
}

size_t Solver::Num_Shapes() 
{ 
  return Shapes.size();
}

int main(int argc, const char *argv[])
{
  Solver solver;

  solver.Read_Grid_And_Shapes(argc, argv);
  
  for (size_t i = 0; i < solver.Num_Shapes(); ++i) { solver.All_Shape_Positions(i); }

  solver.Find_Solution(0);
  solver.Print_Solution();
  
  return 0;
}
