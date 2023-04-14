#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

/* readorig.cpp
   Riley Crockett
   April ??, 2022

   This program...
   */

class Node {    // Create word and die types ----x
  public:
	string name; // for printing dice names ---x
	char c;      // for word characters ---x
	int index;   // The node's index in the graph's vector ---x

	vector <class Edge *> adj;    // A vector of the node's edges
    class Edge* backedge;       // A backedge to the previous node
	vector <bool> letters;     // (allows for node comparisons in 1 check) 
	int visited;
};

class Edge {        
  public:
    Node* from;
    Node* to;

    Edge *reverse;
    int orig;
    int resid;
    // int index;
};

class Graph {
  public:
	int Karp();                 // The path finding Edmonds-Karp algorithm
	void ReadPrint() const;     // The read print function
	void ClearWord();
    
	int num_dice;   // The number of dice
	int num_wlet;   // The number of letters in the word, for removing word nodes

    vector <Node *> nodes;
	Node *source;
	Node *sink;
};

void Graph::ClearWord()
{
  int i;         // Remove the word letter from the back of the vector   
  size_t j;

  nodes.pop_back(); // Remove the sink node from the vector

  for (i = 0; i < num_wlet; i++) nodes.pop_back();

  for (i = 1; i < num_dice+1; i++) nodes[i]->adj.clear();

  num_wlet = 0;    // Reset the word letter count
}

void Graph::ReadPrint() const
{
  size_t i, j;

  for (i = 0; i < nodes.size(); i++) {
    cout << "Node: " << i << ": "; 
	if (nodes[i] == source) {
	  cout << "SOURCE";
	} else if (nodes[i] == sink) {
	  cout << "SINK";
	} else {
	  if (nodes[i]->name != "") {
	    cout << nodes[i]->name;
	  } else {
	    cout << nodes[i]->c;
	  }
	}
	cout << " Edges to";
    for (j = 0; j < nodes[i]->adj.size(); j++) {
	  cout << " " << nodes[i]->adj[j]->to->index;
	}
	cout << endl;
  }
  cout << endl;
}

int main(int argc, char *argv[])
{
  if (argc != 3) return -1;

  ifstream fin;
  string fname;
  
  fname = argv[1];
  fin.open(fname.c_str());
  if (fin.fail()) return -1;

  string line;
  Graph *g = new Graph;
  Node *d;
  Edge *e;
  size_t i;
  int j;

  g->source = new Node;
  g->source->index = 0;
  
  g->nodes.push_back(g->source);  // Push the source to the vector of nodes
  
  g->sink = new Node;  // Create the sink node

  while (getline(fin, line)) {  
    d = new Node;
	d->name = line;            // ----x
	d->letters.resize(256, 0);
    for (i = 0; i < line.length(); i++) d->letters[line[i]] = 1;
	
	e = new Edge;          // Link the die to the source
	e->from = g->source;
	e->to = d;

	e->reverse = new Edge;
    e->reverse->from = d;
    e->reverse->to = g->source;

	g->source->adj.push_back(e);
	d->index = g->nodes.size();  // The node's index in the vector
	g->nodes.push_back(d);
  }

  fin.close();
  fname = argv[2];
  fin.open(fname.c_str());
  if (fin.fail()) return -1;

  Node *let;
  Edge *de;                   // Dice edge, links dice to word letters if they match
  g->num_dice = g->nodes.size()-1;  // The number of dice, excluding the source node

  while (getline(fin, line)) {
	for (i = 0; i < line.length(); i++) {
	  g->num_wlet++;  // Increment the word letter count

	  let = new Node; 
	  let->c = line[i];
	  
	  e = new Edge;          // Link the letter to sink
	  e->from = let;
	  e->to = g->sink;
	  
	  e->reverse = new Edge;
	  e->reverse->from = g->sink;
	  e->reverse->to = let;

      let->index = g->nodes.size();
	  let->adj.push_back(e);
	  
	  // If the letter is on a die, link the die to the letter/vice-versa
	  
	  for (j = 0; j < g->num_dice; j++) {  // Index each of the dice nodes
	    
		if (g->nodes[j+1]->letters[let->c] == 1) {  // If the word letter matches one of the dice letters
		  de = new Edge;
		  de->from = g->nodes[j+1];
		  de->to = let;
		  
		  de->reverse = new Edge;
          de->reverse->from = let;
          de->reverse->to = g->nodes[j+1];
	      
		  let->backedge = de->reverse;
		  g->nodes[j+1]->adj.push_back(de);  // Push the letter to the die's adjacency list 
		}
	  }

	  g->nodes.push_back(let); 
    }

	g->sink->index = g->nodes.size();
	g->nodes.push_back(g->sink);        // After reading letters, push the sink node to the vector
   
	g->ReadPrint();
	g->ClearWord();    // Clear the word from the graph, for the next iteration    (FREE MEMORY?)
  }

  fin.close();

  // Delete the allocated memory ---x

  return 0;
}
