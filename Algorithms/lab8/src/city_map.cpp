#include "city_map.hpp"
#include <vector>
#include <math.h>
using namespace std;

/* city_map.cpp
   Riley Crockett
   April 10, 2022

   This program defines two functions from the city_map.hpp header file.
   
   City_Map() - The constructor for the City_Map() class. It reads intersections
     from std input, stores their information, and creates roads between them.

   Dijkstra() - A function that implements Dijkstra's algorithm to determine
     the fastest route from the first intersection to the last.
   */

City_Map::City_Map()
{
  Intersection *isect;       // An intersection pointer
  int mxs = -1, mxa = -1;    // Integers for saving the highest street and avenue
  int i, j;  

  while (!cin.fail()) {    /* Read from std input */
    isect = new Intersection;    // Create a new intersection
	
	/* Store the intersection information in the pointer */

	cin >> isect->street >> isect->avenue >> isect->x 
		>> isect->y >> isect->green[0] >> isect->green[1];
    
	/* After reading an intersection */

	if (!cin.fail()) {
	  all.push_back(isect);    // Push the intersection to the list of all intersections

	  /* Store the first and last intersections if the conditions are met */

	  if (isect->street == 0 && isect->avenue == 0) first = isect;
	  if (mxs <= isect->street || mxa <= isect->avenue) {
	    mxs = isect->street;
	    mxa = isect->avenue;
	    last = isect;
	  }
	}
  }

  /* Create a temporary 2d vector for indexing intersections.
     (The outer vector is indexed by street, and the inner by avenue) */

  vector< vector<Intersection *> > tmp;
  tmp.resize(mxs+1, vector <Intersection *>(mxa+1) );
  list <Intersection *>::iterator ait;
  for (ait = all.begin(); ait != all.end(); ait++) {
    tmp[(*ait)->street][(*ait)->avenue] = (*ait);
  }

  Road_Segment *rs;    // A road segment pointer
  
  for (i = 0; i < (int)tmp.size(); i++) {     // Loop through the streets 
	for (j = 0; j < (int)tmp[i].size(); j++) {    // Loop through the avenues
	
	  if (j > 0 && (i % 5 == 0 || i % 2 == 1)) {    // Add left streets
	    rs = new Road_Segment;
        rs->type = STREET;
        rs->number = i;
		rs->from = tmp[i][j];
		rs->to = tmp[i][j-1];
		tmp[i][j]->adj.push_back(rs);
	  }
	  if (j < mxa && (i % 5 == 0 || i % 2 == 0)) {    // Add right streets
        rs = new Road_Segment;
        rs->type = STREET;
        rs->number = i;
		rs->from = tmp[i][j];
		rs->to = tmp[i][j+1];
		tmp[i][j]->adj.push_back(rs);
	  }
	  if (i > 0 && (j == mxa || j % 5 == 0 || j % 2 == 1)) {    // Add down avenues
	    rs = new Road_Segment;
        rs->type = AVENUE;
        rs->number = j;
        rs->from = tmp[i][j];
		rs->to = tmp[i-1][j];  
		tmp[i][j]->adj.push_back(rs);
	  }
	  if (i < mxs && (j == mxa || j % 2 == 0 || j % 5 == 0)) {    // Add up avenues
        rs = new Road_Segment;
        rs->type = AVENUE;
        rs->number = j;
        rs->from = tmp[i][j];
		rs->to = tmp[i+1][j];
		tmp[i][j]->adj.push_back(rs);
	  }
	}
  }
  
  list <Road_Segment *>::iterator adjit;    // An iterator for indexing an intersection's roads
  double dx, dy;                            // Doubles for distance in the x and y directions

  for (ait = all.begin(); ait != all.end(); ait++) {
	for (adjit = (*ait)->adj.begin(); adjit != (*ait)->adj.end(); adjit++) {
      dx = (*adjit)->to->x - (*adjit)->from->x;      // Calculate the distance in the x 
	  dy = (*adjit)->to->y - (*adjit)->from->y;      // Calculate the distance in the y
	  
	  /* Set the road's distance to the distance between its two intersections */

	  (*adjit)->distance = sqrt((dx*dx)+(dy*dy));    
    }
  }
}

double City_Map::Dijkstra(int avg_best_worst)
{
  list <Intersection *>::iterator ait;    // An iterator for traversing all intersections

  for (ait = all.begin(); ait != all.end(); ait++) { 
	(*ait)->backedge = NULL;        // Set every backlink to NULL
	(*ait)->best_time = -1;         // Set the best time / visited field to -1
  }
 
  list <Road_Segment *>::iterator adjit;    // An iterator for roads/edges
  Intersection *n;   // An intersection pointer
  double d = 0;      // A double for the time between intersections

  first->best_time = 0;
  bfsq.insert(make_pair(d, first));  // Set first's best time to 0 and put it on the multimap

  while (!bfsq.empty()) {          // Keep looping until the multimap is empty
	n = (*bfsq.begin()).second;    // Set n to the intersection at the front of the multimap
    bfsq.erase(bfsq.begin());      // Remove the first entry from the multimap
    d = 0;                         // Reset the time

    /* Loop through the intersection's roads */

	for (adjit = n->adj.begin(); adjit != n->adj.end(); adjit++) {
	  
	  /* Set d to the previous time plus the road's time */

	  d = n->best_time + (*adjit)->distance/30 * 3600;

	  if (avg_best_worst == 'W') {    // Add the worst light times to d
		d += ((*adjit)->type == STREET) ? (*adjit)->to->green[1] : (*adjit)->to->green[0];
	  } 
	  else if (avg_best_worst == 'A') {    // Add the average of the best and worst times to d
	    if ((*adjit)->type == STREET) {
	      d += (pow((*adjit)->to->green[1], 2)) / (2*((*adjit)->to->green[1] + (*adjit)->to->green[0]));
		} else {
		  d += (pow((*adjit)->to->green[0], 2)) / (2*((*adjit)->to->green[0] + (*adjit)->to->green[1]));
	 	}
	  }

	  /* If the next intersection hasn't been visited, or this road yields a faster time */

      if ((*adjit)->to->best_time == -1 || d < (*adjit)->to->best_time) {
		(*adjit)->to->best_time = d;                // Set the next intersection's time to d
        (*adjit)->to->backedge = *adjit;            // Set the next intersection's backedge to the road
        bfsq.insert(make_pair(d, (*adjit)->to));    // Insert the road into the multimap
	  } 
    }
  }

  Road_Segment *ps;       // A road segment pointer for traversing backedges
  ps = last->backedge;    // Initialize the path segment to last

  while (ps != first->backedge) {   // Loop until reaching first's backedge
    path.push_front(ps);            // Push the backedge to the path
	ps = ps->from->backedge;        // Set the path segment to the next backedge
  }

  return d;    // Return the shortest path time
}
