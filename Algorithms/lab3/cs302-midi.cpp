#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "cs302-midi.h"
using namespace std;

/* cs302-midi.cpp
   Riley Crockett
   February 21, 2022

   This program implements two methods that convert MIDI file 
   representations to each other : Event to Note / Note to Event */

void CS302_Midi::el_to_nd()
{
  vector <ND *> nv(128, NULL);    // A vector for note pointers indexed on pitch
  EventList::iterator eit;        // The event list iterator
  double ttime = 0.0;             // A double for the total time elapsed
  nd = new NDMap;                 // A new note map
  Event *e;                       // An event pointer
  ND* d;                          // A note pointer for DAMPER notes

  /* Traverse the event list */
  for (eit = el->begin(); eit != el->end(); eit++) {
    e = *eit;                               // Derefence the event 
    ttime += ((double) e->time) / 480.0;    // Increment total time by event time

	if (e->key == 'O') {
	  ND *n = new ND;            // On an 'ON' event create a note
	  n->key = 'N';
	  n->start = ttime;          // Set the note's start time to the total time
	  n->pitch = e->v1;
	  n->volume = e->v2;
	  nv[n->pitch] = n;          // Put the note in the vector (at index = pitch)
	} else if (e->key == 'F') {
	  /* On an 'OFF' event find the pitch in the vector. Set that note's 
	     stop time, then insert the note into the map. */	  
	  nv[e->v1]->stop = ttime;   
	  nd->insert(make_pair(nv[e->v1]->start, nv[e->v1]));
	} else {
	  if (e->v1 == 1) {
		d = new ND;          // On a 'DAMPER UP' event create a note instance
		d->key = e->key;
		d->start = ttime;
	  } else {
        d->stop = ttime;                       // On 'DAMPER DOWN' set the stop time
		nd->insert(make_pair(d->start, d));    // Insert the damper into the map
	  }
	}
  }
}

void CS302_Midi::nd_to_el()
{
  NDMap::iterator nit;        // The note map iterator
  int srt, end, prevt = 0;    // Integers for start, end, and previous times
  int on_rank, off_rank;      // Used to order multimap entries
  el = new EventList;         // A new event list
  multimap <int, Event *> im;                // 'Inner' multimap keyed by rank
  map <int, multimap <int, Event *> > om;    // 'Outer' map keyed on time
  multimap <int, Event *>::iterator imit;                // 'Inner' iterator
  map <int, multimap <int, Event *> >::iterator omit;    // 'Outer' iterator

  /* Traverse the note map */
  for (nit = nd->begin(); nit != nd->end(); nit++) {
	srt = rint(nit->second->start * 480.0);    // Calculate the start time
	end = rint(nit->second->stop * 480.0);     // Calculate the end time
    if (srt == end) continue;                  // If times are the same ignore the note
	
	/* Note: The rank variables determine the order of events at the same time. 
	   - Rank values are as follows : (OFF, DAMPER UP, DAMPER DOWN, ON) = (0, 1, 2, 3) */

	/* Create two events for each note and set their values */
	Event *on = new Event;
	Event *off = new Event;
	if (nit->second->key == 'N') {
	  on->key = 'O';
	  off->key = 'F';
	  on->v1 = off->v1 = nit->second->pitch;
      on->v2 = nit->second->volume;
	  off_rank = 0;
	  on_rank = 3;
	} else {
	  on->key = off->key = 'D';
      on->v1 = 1;                
      off->v1 = 0;               
	  off_rank = 1;
	  on_rank = 2;
	}

	/* See if there is an outer map entry at the start/end time. If so, insert an event 
	   pair to the inner map at that time. Otherwise make a new outer map entry. */
	omit = om.find(srt);
    if (omit != om.end()) {
	  omit->second.insert(make_pair(on_rank, on));
	} else {
	  im.clear();
	  im.insert(make_pair(on_rank, on));
	  om.insert(make_pair(srt, im));
	} 
	omit = om.find(end);
    if (omit != om.end()) {
      omit->second.insert(make_pair(off_rank, off));
    } else {
      im.clear();
      im.insert(make_pair(off_rank, off));
      om.insert(make_pair(end, im));
    }
  }

  /* Loop through the inner and outer maps */
  for (omit = om.begin(); omit != om.end(); omit++) {
	for (imit = omit->second.begin(); imit != omit->second.end(); imit++) {	  
	  /* Set the event's time to the difference between the current and previous times */
	  imit->second->time = omit->first - prevt;
	  el->push_back(imit->second);    // Push the event to the list
	  prevt = omit->first;            // Set the previous time to the current time
	}
  }
}
