#include "ss_solver.hpp"
using namespace std;

/* ss_solver.cpp
   Riley Crockett
   November 9, 2021
   
   This program...
   */

bool Solver::Read_Grid_And_Shapes(int argc, const char *argv[])
{
  string line, shape_comp;
  istringstream ss;
  vector <string> shape;
  string ones; 
  size_t i;

  for (i = 1; i < argc; ++i) { grid.push_back(argv[i]); }

  while (getline(cin, line)) {
    ss.clear(); 
	ss.str(line);
	while (ss >> shape_comp) { shape.push_back(shape_comp); }
	shapes.push_back(shape); 
	shape.clear();
  }
  for (i = 0; i < grid[0].size(); ++i) { ones += "1"; }
  for (i = 0; i < grid.size(); ++i) { full_grid.push_back(ones); }
  return true;
}

bool Solver::Place_Shape(size_t index, size_t r, size_t c)
{
  size_t i, j;

  if (grid.size() < r + shapes[index].size()) return false;

  for (i = 0; i < shapes[index].size(); ++i) {
	
	if (grid[r].size() < c + shapes[index][i].size()) return false;

	for (j = 0; j < shapes[index][i].size(); ++j) {
      
	  if (shapes[index][i][j] == '1') {
        grid[r + i][c + j] = (grid[r + i][c + j] == '1') ? '0' : '1';
      }
    }
  }
  return true;
}

void Solver::All_Shape_Positions(size_t index)
{
  size_t i, j;
  vector <int> rows, cols;
  
  for (i = 0; i < grid.size(); ++i) {
    for (j = 0; j < grid[i].size(); ++j) {

	  if (Place_Shape(index, i, j)) {
        rows.push_back(i); cols.push_back(j);
        Place_Shape(index, i, j);
      }
    }
  }
  valid_rows.push_back(rows); valid_cols.push_back(cols);
}

bool Solver::Find_Solution(size_t index)
{
  size_t i;
  
  if (index == shapes.size()) {
    if (grid == full_grid) { return true; } return false;
  }

  for (i = 0; i < valid_rows[index].size(); ++i) {

    Place_Shape(index, valid_rows[index][i], valid_cols[index][i]);
    solution_rows.push_back(valid_rows[index][i]);
    solution_cols.push_back(valid_cols[index][i]);
    
    if (Find_Solution(index + 1)) return true; 
    
    solution_rows.pop_back(); 
	solution_cols.pop_back();
	Place_Shape(index, valid_rows[index][i], valid_cols[index][i]);
  }
  return false;
}

void Solver::Print_Solution() const
{
  size_t i, j;

  if (solution_rows.empty()) return;

  for (i = 0; i < shapes.size(); ++i) {
	for (j = 0; j < shapes[i].size(); ++j) { cout << shapes[i][j] << " "; }
	cout << solution_rows[i] << " " << solution_cols[i] << endl;
  }
}

size_t Solver::Num_Shapes() { return shapes.size(); }

int main(int argc, const char *argv[])
{
  Solver solver;

  if (!solver.Read_Grid_And_Shapes(argc, argv)) return 1;

  for (size_t i = 0; i < solver.Num_Shapes(); ++i) { solver.All_Shape_Positions(i); }
  
  solver.Find_Solution(0);
  solver.Print_Solution();
  return 0;
}
