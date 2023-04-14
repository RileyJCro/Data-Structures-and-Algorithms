#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* maze_solve.cpp
   Riley Crockett
   April 4th, 2022

   This program reads in a maze, converts it into a graph with nodes/edges, and 
   prints the path from the start to the end using depth-first-search.
   */

class Node {    // A class to represent a cell in the maze
  public:
	int visited;           // An int that is -1 if the node is unvisited
	vector <int> edges;    // A vector for adjacent cells
};

class Graph {    // A class to hold the nodes/cells of the maze
  public:
	vector <Node *> nodes;                           // A vector for all of the nodes
	bool DFS_Print(int index, vector <int> path);    // The depth-first-search print
};

bool Graph::DFS_Print(int index, vector <int> path)
{
  size_t i, j;
  Node *n = nodes[index];    // A pointer to the node at the current index

  if (index == nodes.size()-1) {    // Base case: Print after reaching the last cell
    for (i = 0; i < path.size(); i++) cout << "PATH " << path[i] << endl;
    return true;
  }

  /* Loop through the node's edges. If the edge is unvisited, set the visited field, 
     push the edge onto the path, and recursively call on the edge. If the path reaches 
	 the end, return true, otherwise pop the edge from the path. If the edge has been 
	 visited and there are no other unvisited edges, return false. */

  for (i = n->edges.size()-1; i >= 0; i--) {
	if (nodes[n->edges[i]]->visited == -1) {
      n->visited = index;         
	  path.push_back(n->edges[i]);
	  if (DFS_Print(n->edges[i], path)) return true;
	  path.pop_back();
    } else if (i != 0) {
	  for (j = 0; j < i; j++) {
	    if (nodes[n->edges[j]]->visited != -1) return false;
	  }
	}
  }
}

int main()
{
  size_t i;
  string str;     // A string for reading maze keywords
  int r, c;       // The number of rows and column
  Graph g;        // An instance of the graph class
  Node *n;        // A node class pointer
  int w1, w2;     // The wall indices

  cin >> str; if (str != "ROWS") return -1;    // Read in the rows and columns then print
  cin >> r;
  cin >> str; if (str != "COLS") return -1;
  cin >> c;
  cout << "ROWS " << r << " COLS " << c << endl;
  
  for (i = 0; i < r*c; i++) {        // Create r*c nodes
    n = new Node;
	n->visited = -1;         // Set the node's visited field to unvisited
	g.nodes.push_back(n);    // Push the node onto the graph

	/* Check if the index is at the top, left, right, and bottom sides of the maze 
	   to determine possible edges. (The order is by index from small to large). */

	if (i >= c) g.nodes[i]->edges.push_back(i-c);
	if (i % c != 0) g.nodes[i]->edges.push_back(i-1);
	if ((i+1) % c != 0) g.nodes[i]->edges.push_back(i+1);
	if (i+c <= (r*c)-1) g.nodes[i]->edges.push_back(i+c);
  }

  while (!cin.fail()) {
    cin >> str >> w1 >> w2;    // Read in the wall indices
	if (!cin.fail()) {
	  if (str != "WALL") continue;
	  cout << "WALL " << w1 << " " << w2 << endl;    // Print the wall
      
	  /* Go through the edges of one wall index. 
	     If it matches the other wall index erase the edge */

	  for (i = 0; i < g.nodes[w1]->edges.size(); i++) {
	    if (g.nodes[w1]->edges[i] == w2) {
		  g.nodes[w1]->edges.erase(g.nodes[w1]->edges.begin() + i);
	      break;
		}
	  }
	  for (i = 0; i < g.nodes[w2]->edges.size(); i++) {
        if (g.nodes[w2]->edges[i] == w1) {
          g.nodes[w2]->edges.erase(g.nodes[w2]->edges.begin() + i);
          break;
        }
      } 
    }
  }

  vector <int> path = {0};    // A vector to hold the path from node 0 to node r*c-1
  g.DFS_Print(0, path);       // Call the DFS print function on the graph

  for (i = 0; i < g.nodes.size(); i++) delete g.nodes[i];
  return 0;
}
