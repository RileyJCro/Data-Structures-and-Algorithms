#include "dlist.hpp"
#include <iostream>

using namespace std;

/*	dlist.cpp
    Riley Crockett
    November 2, 2021

	This program implements the methods of an API for double linked lists, declared in
	the dlist.hpp header file. The Insert_Before() and Erase() methods are defined 
	at the bottom of the file, and are implemented in many other methods.
	*/

/* Default constructor */

Dlist::Dlist()
{
  Dnode *sn = new Dnode;         // A Dnode pointer to allocated memory for the sentinel node
  this->size = 0;                // Initialize the list's size
  this->sentinel = sn;           // Set the sentinel node to the allocated memory
  this->sentinel->flink = sn;    // Set the sentinel's front pointer to itself
  this->sentinel->blink = sn;    // Set the sentinel's back pointer to itself
}

/* Copy constructor */

Dlist::Dlist(const Dlist &d)
{
  Dnode *n;                      // A Dnode pointer for the copied values
  Dnode *sn = new Dnode;         // A Dnode pointer to allocated memory for the sentinel node

  /* Create the sentinel node */

  this->size = 0;         
  this->sentinel = sn;
  this->sentinel->flink = sn;
  this->sentinel->blink = sn;
  
  /* Copy the original list's elements */

  for (n = d.Begin(); n != d.End(); n = n->Next()) this->Push_Back(n->s);
}

/* Assignment overload */

Dlist& Dlist::operator= (const Dlist &d)
{
  Dnode *n;        // A Dnode pointer for the copied values
  Clear();         // Clear all of the list's nodes, except for the sentinel

  /* Copy the original list's elements */
   
  for (n = d.Begin(); n != d.End(); n = n->Next()) {
    Push_Back(n->s);
  }
  return *this;    // Return a refernce to this instance
}

/* Destructor */

Dlist::~Dlist()
{
  // delete this->sentinel;
  delete sentinel;
  sentinel = NULL;
}

void Dlist::Clear()
{
  /* Erase list elements from the beginning until empty */

  while (!Empty()) (void) Erase(Begin());
}

bool Dlist::Empty() const
{
  /* Return true if the size is 0 */

  return (size == 0);  
}

size_t Dlist::Size() const
{
  /* Return the size of the list */

  return size;
}

void Dlist::Push_Front(const string &s)
{
  /* Put a new node at the front of the list */

  this->Insert_Before(s, Begin());
}

void Dlist::Push_Back(const string &s)
{
  /* Put a new node at the back of the list */

  this->Insert_Before(s, End()); 
}

string Dlist::Pop_Front()
{
  /* Remove and return the first element of the list */

  string fs;                 // A string to store the first node's string 

  if (Empty()) return "";    // Return an empty string if the list is empty
  fs = Begin()->s;           // Store the first node's string
  Erase(Begin());            // Erase the first node 
  return fs;
}

string Dlist::Pop_Back()
{
  /* Remove and return the last element of the list */

  string bs;                 // A string to store the last node's string
  
  if (Empty()) return "";    // Return an empty string if the list is empty
  bs = Rbegin()->s;          // Store the last node's string
  Erase(Rbegin());           // Erase the last node
  return bs;
}

Dnode* Dlist::Begin() const
{
  return sentinel->flink;    // Return a pointer to the first node
}

Dnode* Dlist::End() const
{
  return sentinel;    // Return a pointer to "one past" the last node
}

Dnode* Dlist::Rbegin() const
{
  return sentinel->blink;    // Return a pointer to the last node
}

Dnode* Dlist::Rend() const
{
  return sentinel;    // Return a pointer to "one before the first node
}

Dnode* Dnode::Next()
{
  return this->flink;    // Return a pointer to the current node's flink
}

Dnode* Dnode::Prev()
{
  return this->blink;   // Return a pointer to the current node's blink
}

void Dlist::Insert_Before(const string &s, Dnode *n)
{
  /* Insert a node before the current node */

  Dnode *prev;              // A Dnode pointer for the previous node
  Dnode *newnode;           // A Dnode pointer for the new node
  
  newnode = new Dnode;      // Allocate memory for the new node
  newnode->s = s;           // Set the string for the new node

  prev = n->blink;          // Set prev to the node before the specified node

  newnode->blink = prev;    // Set new node's back pointer to prev
  newnode->flink = n;       // Set new node's front pointer to the specified node

  prev->flink = newnode;    // Set prev's front pointer to the new node 

  n->blink = newnode;       // Set the specified node's back pointer to the new node

  size++;                   // Increase the list's size
}

void Dlist::Insert_After(const string &s, Dnode *n)
{
  Insert_Before(s, n->Next());    // Insert a node after the current node
}

void Dlist::Erase(Dnode *n)
{
  /* Erase the current node */
  
  Dnode *prev;             // A Dnode pointer for the previous node
  Dnode *next;             // A Dnode pointer to the next node
  
  if (n != sentinel) {     // Make sure that the specified node is not the sentinel
    prev = n->Prev();      
    next = n->Next();

	prev->flink = next;    // Set prev's front pointer to one past the specified node
	next->blink = prev;    // Set next's back pointer to one before the specified node

	delete n;              // Free the memory pointed to by n
    n = NULL;              // Set n to a NULL pointer
	size--;                // Decrement the list's size
  }
}	
