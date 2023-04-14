#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

vector <string> grid;
vector < vector <string> > shapes;

vector <int> solution_rows;
vector <int> solution_cols;
//--------------------------------------------
vector < vector <int> > valid_rows;
vector < vector <int> > valid_cols;
vector <string> full_grid;

bool Read_Grid_And_Shapes(int argc, const char *argv[])
{
  int i;
  istringstream ss;
  string line;
  string shape_cpnt;
  vector <string> shape;

  for (i = 1; i < argc; ++i) grid.push_back(argv[i]);

  while (getline(cin, line)) {
    ss.clear(); 
	ss.str(line);
	while (ss >> shape_cpnt) { shape.push_back(shape_cpnt); }
	shapes.push_back(shape); 
	shape.clear();
  }
  //----------------------grid with all 1's
  string ones; 
  size_t j;
  for (j = 0; j < grid[0].size(); ++j) ones += "1";
  for (j = 0; j < grid.size(); ++j) full_grid.push_back(ones);
  
  return true;
}

bool Place_Shape(size_t index, size_t r, size_t c)
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

void All_Shape_Positions(size_t index)
{
  size_t i, j;
  vector <int> rows;
  vector <int> cols;

  for (i = 0; i < grid.size(); ++i) {
    for (j = 0; j < grid[i].size(); ++j) {
      
	  if (Place_Shape(index, i, j)) {
		rows.push_back(i);
		cols.push_back(j);
        Place_Shape(index, i, j);
      }
    }

  }	
  valid_rows.push_back(rows);
  valid_cols.push_back(cols);
}

bool Find_Solution(size_t index)
{
  size_t i;
  
  if (index == shapes.size()) {
    if (grid == full_grid) return true;
	return false;
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

void Print_Solution()
{
  size_t i, j;

  if (solution_rows.empty()) return;

  for (i = 0; i < shapes.size(); ++i) {
	for (j = 0; j < shapes[i].size(); ++j) { cout << shapes[i][j] << " "; }
	cout << solution_rows[i] << " " << solution_cols[i] << endl;
  }
}

int main(int argc, const char *argv[])
{
  size_t i;
  
  if (!Read_Grid_And_Shapes(argc, argv)) return 1;
    	
  for (i = 0; i < shapes.size(); ++i) { All_Shape_Positions(i); }

  Find_Solution(0);
  Print_Solution();
  return 0;
}
