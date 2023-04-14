#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

/* worddice.cpp 
   Riley Crockett
   April 25, 2022

   This program uses the Edmonds-Karp algorithm for maximum flow to test and construct 
   possible spellings from a set of dice to a set of letters.
   */

class Node {
  public:
	Node(string name, int num_nodes) { 
	  this->name = name;
	  this->c = '0';
	  this->backedge = NULL;
	  this->index = num_nodes;
	  this->visited = 0;
	}
    int visited;    // A visited field for BFS
	string name;    // The node's name
	char c;         // A character for dice letters
	int index;      // The node's index in the graph's node vector

	vector <class Edge *> adj;    // A vector of forward edges
    class Edge* backedge;         // A backedge used to set residuals
	vector <bool> letters;	      // For dice nodes (indices are set to 1 at its char values)
    
	vector <int> dice_match;      
};

class Edge {        
  public:
	Edge(Node* from, Node* to) {
	  this->from = from;
	  this->to = to;
	  if (this->from->name == "SOURCE" || this->to->name == "SINK") {
	    from->adj.push_back(this); 
	  } 
	}
    Node* from;       // The edge's initial node
    Node* to;         // The edge's terminal node
    Edge *reverse;    // Flips the from and to nodes
	int orig;         // The original/capacity of the edge
	int rsid;         // The residual capacity of the edge
};

class Graph {
  public:
    Graph() {
	  this->source = new Node("SOURCE", 0);
      this->sink = new Node("SINK", -1); 
	  this->nodes.push_back(source);
	}
	int BFS();           // The Breadth-First-Search function
	int FindPaths();     // Calls BFS to find max flow
	void ClearHalf();    // Clears/resets values in between words
	size_t num_dice;     // The number of dice
	size_t num_lets;     // The number of word letters
    Node *source;
    Node *sink;

	vector <Node *> nodes;           // All of the nodes in the graph
    vector <int> path;               // A path from source to sink
	vector < vector<int> > paths;    // Stores the paths returned by BFS
	vector <int> matches;            // Used for printing the spellings
};

void Graph::ClearHalf()
{
  size_t i; 

  /* Pop the source node and clear the letters from the graph's nodes */

  nodes.pop_back();
  for (i = 0; i < num_lets; i++) nodes.pop_back();

  /* Reset the number of letters, clear the spelling matches, and clear the dice adjancies */

  num_lets = 0;
  matches.clear();
  for (i = 1; i <= num_dice; i++) nodes[i]->adj.clear();

  /* Reset the capacities for the source node */

  for (i = 0; i < nodes[0]->adj.size(); i++) {
    nodes[0]->adj[i]->orig = 1; nodes[0]->adj[i]->rsid = 0;
	nodes[0]->adj[i]->reverse->orig = 0; nodes[0]->adj[i]->reverse->rsid = 1;  
    nodes[0]->adj[i]->reverse->reverse->orig = 1;
	nodes[0]->adj[i]->reverse->reverse->rsid = 0;
  }
}

int Graph::BFS()
{
  size_t i;
  Node *n;
  queue <Node *> q;

  /* Set every node's backedge to null and visited to 0 */

  for (i = 0; i < nodes.size(); i++) {
    nodes[i]->backedge = NULL;
    nodes[i]->visited = 0;	
  } 

  source->visited = 1;    // Set the source's visited field
  q.push(source);         // Push the source to the queue

  /* Loop until the queue is empty. Set n to frontmost node in the queue 
     and pop it. Loop through the node's adjacency list. If the next node 
	 is unvisited and has capacity set its visited field, backedge, and
	 push the next node onto the queue. */

  while (!q.empty()) {
	n = q.front();
	q.pop();
   
	for (i = 0; i < n->adj.size(); i++) {   	  
	  // if (n->adj[i]->to->visited == 0 && n->adj[i]->orig >= 1) {
	  if (n->adj[i]->to->visited == 0 && n->adj[i]->orig - n->adj[i]->rsid > 0) {
		n->adj[i]->to->visited = 1;
		n->adj[i]->to->backedge = n->adj[i]->reverse;
		if (n->adj[i]->to == sink) break;
        q.push(n->adj[i]->to);
	  } 
	} 
  }

  /* If there isn't a path from the source to the sink return 0 */
  
  if (sink->backedge == NULL) return 0;
  return 1;
}

int Graph::FindPaths()
{
  size_t i, j;
  Edge *e;

  if (num_dice < num_lets) return 0;

  /* Clears the paths */

  paths.clear();
  paths.resize(num_lets, vector <int>(0));

  for (i = 1; i <= num_dice; i++) {
    source->adj[i-1]->orig = nodes[i]->adj.size();
	source->adj[i-1]->rsid = 0;
    source->adj[i-1]->reverse->orig = 0;
	source->adj[i-1]->reverse->rsid = nodes[i]->adj.size();	
  }
/*
  for (i = num_dice+1; i < nodes.size()-1; i++) {
    nodes[i]->adj[0]->orig = nodes[i]->dice_match.size();
    source->adj[i-1]->rsid = 0;
    source->adj[i-1]->reverse->orig = 0;
    source->adj[i-1]->reverse->rsid = nodes[i]->adj.size();
  }
*/
  int flow;
  int max_flow = 0;

  /* Loop until no more augmented paths can be found. Set update the edge capacities 
     and store the paths. */
  
  while (true) {  
	flow = BFS();
	if (flow == 0) break;
	max_flow += flow;

    e = sink->backedge->to->backedge;
    path.push_back(sink->index);

	while (e != NULL) {
      // e->reverse->orig -= flow; e->reverse->rsid += flow;
	  // e->orig += flow; e->rsid -= flow;
      e->reverse->rsid += flow;
	  e->rsid -= flow;

      path.push_back(e->from->index);
      e = e->to->backedge;
    }
    paths[path[1] - num_dice - 1].push_back(path[2]);
	path.clear();
  }

  for (i = 0; i < paths.size(); i++) {
	if (paths[i].size() == 0) return 0;
    if (paths[i].size() == 1) {
	  for (j = 0; j < paths.size(); j++) {
	    if (j != i && paths[j].size() == 1) {
	      if (paths[i][0] == paths[j][0]) return 0;
		}
	  }
	}
  }

  int cur_connect = 0;
  int min_connect = 1000;
  int min_index;

  while (true) {
    for (i = 0; i < paths.size(); i++) {
	  if (paths[i].size() == 0) continue;

	  if (paths[i].size() == 1) {
		matches[i] = paths[i][0]-1;
   	    paths[i].pop_back();
        for (j = 0; j < paths.size(); j++) {
          for (size_t k = 0; k < paths[j].size(); k++) {
            if (paths[j][k] == matches[i]+1) {
              paths[j].erase(paths[j].begin()+k);
            }
          }
        }
	    i = 0;
	  } 
	}
	for (i = 0; i < matches.size(); i++) if (matches[i] == -1) break;
    if (i == matches.size()) return 1;
	if (paths[i].size() == 0) return 0;
	
	for (i = 0; i < paths.size(); i++) {
      if (paths[i].size() == 0 || paths[i].size() == 1) continue;
	  min_connect = 1000;
      for (j = 0; j < paths[i].size(); j++) {
        cur_connect = 0;
        for (size_t k = 0; k < paths.size(); k++) {
          for (size_t l = 0; l < paths[k].size(); l++) {
            if (paths[k][l] == paths[i][j]) cur_connect++;
          }
        }
        if (cur_connect < min_connect) {
          min_connect = cur_connect;
          min_index = j;
        }
        if (j == paths[i].size()-1) {
          matches[i] = paths[i][min_index]-1;
		  paths[i].clear();
          break;
        }
      }

      for (j = 0; j < paths.size(); j++) {
        for (size_t k = 0; k < paths[j].size(); k++) {
          if (paths[j][k] == matches[i]+1) {
            paths[j].erase(paths[j].begin()+k);
            break;
          }
        }
      }
	  break;
	}	  
  }
  
  return 1;
}

int main(int argc, char *argv[])
{
  if (argc != 3) return -1;
  size_t i, j;
  ifstream fin;
  string file_name, line;

  file_name = argv[1];
  fin.open(file_name.c_str());    // Open the dice file
  if (fin.fail()) return -1;

  Graph *g = new Graph;    // An instance of the graph class
  Node *d;                 // A pointer to a dice node
  Edge *e;                 // A pointer to an edge

  /* Loop for every line of the dice file. Create a dice node for each line
     and link them to the source node. Then push the dice node onto the graph's nodes. */

  while (getline(fin, line)) {  
    d = new Node(line, g->nodes.size());
    d->letters.resize(256, 0);
    for (i = 0; i < line.length(); i++) d->letters[line[i]] = 1;

    e = new Edge(g->source, d);
	e->orig = 1; e->rsid = 0;
	e->reverse = new Edge(d, g->source);
    e->reverse->orig = 0; e->reverse->rsid = 1;
	e->reverse->reverse = e;
	
	g->nodes.push_back(d);
  }

  fin.close();                    // Close the dice file and open the words file
  file_name = argv[2];
  fin.open(file_name.c_str());
  if (fin.fail()) return -1;
  g->num_dice = g->nodes.size()-1;    // Store the number of dice

  Node *let;    // A node pointer for a word letter
  Edge *de;     // An edge pointer linking dice to letters
  
  /* Each line of the words file is a word. Create a node for each letter in the word 
     and link them to the sink node. */

  while (getline(fin, line)) {
	for (i = 0; i < line.length(); i++) {
	  g->num_lets++;
	  let = new Node(line, g->nodes.size());
	  let->c = line[i];

	  e = new Edge(let, g->sink);
	  e->orig = 1; e->rsid = 0;
	  e->reverse = new Edge(g->sink, let);
	  e->reverse->orig = 0; e->reverse->rsid = 1;
	  e->reverse->reverse = e;
	  
	  for (j = 1; j <= g->num_dice; j++) {          // Index each of the dice nodes
		if (g->nodes[j]->letters[let->c] == 1) {    // If the word letter matches one of the dice letters
		  de = new Edge(g->nodes[j], let);
		  de->orig = 1; de->rsid = 0;
		  de->reverse = new Edge(let, g->nodes[j]);    
		  de->reverse->orig = 0; de->reverse->rsid = 1;
		  de->reverse->reverse = de;
		  g->nodes[j]->adj.push_back(de);           // Push the letter to the die's adjacency list     
		} 	
	  }
	  g->nodes.push_back(let);    // Push the letters onto the graph's nodes
    }

	g->sink->index = g->nodes.size();      // Set the sinks index in the graph's nodes
	g->nodes.push_back(g->sink);           // After reading letters, push the sink node to the vector   
	g->matches.resize(g->num_lets, -1);    // Resize the matches vector to the number of letters

	/* If there is a valid spelling print it. Otherwise print that you cannot spell the word. */

	if (g->FindPaths()) {	
	  for (i = 0; i < g->matches.size(); i++) {
	    cout << g->matches[i];
		(i != g->matches.size()-1) ? cout << "," : cout << ": ";
	  }  
	} else {
	  cout << "Cannot spell ";
	}
	cout << line << endl;
	
	g->ClearHalf();    // Clears the letters from the graph before reading the next word
  }

  fin.close();

  return 0;
}
