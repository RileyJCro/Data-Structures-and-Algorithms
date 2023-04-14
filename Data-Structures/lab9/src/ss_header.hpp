#include <iostream>
#include <sstream>
#include <vector>

class Solver {  
  public:
    
	bool Read_Grid_And_Shapes(int argc, const char *argv[]);
	
	bool Place_Shape(size_t index, size_t r, size_t c);
    
	void All_Shape_Positions(size_t index);
	
	bool Find_Solution(size_t index);
	
	void Print_Solution() const;
	
	size_t Num_Shapes();
  
  protected:
	
	std::vector <std::string> grid;
	std::vector <std::string> full_grid;
    
	std::vector < std::vector <std::string> > shapes;
    
	std::vector < std::vector <int> > valid_rows;
	std::vector < std::vector <int> > valid_cols;
	
	std::vector <int> solution_rows;
	std::vector <int> solution_cols;
};
