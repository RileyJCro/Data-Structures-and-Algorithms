#include "dlist.hpp"
#include <iostream>

using namespace std;

/*	dlist.cpp
    Riley Crockett
    November ??, 2021

	This program...
	*/

// do you need all of the this's

/* The default constructor */
Dlist::Dlist()
{
  Dnode *snode = new Dnode;
  this->size = 0;
  this->sentinel = snode;
  this->sentinel->s = "";          // do you need to initialize the sentinel string?
  this->sentinel->flink = snode;   
  this->sentinel->blink = snode;
}

/* The copy constructor */

Dlist::Dlist(const Dlist &d)                // copies AND initializes new objects
{
  Dnode *node;
  Dnode *snode = new Dnode;
  this->size = 0;
  this->sentinel = snode;
  this->sentinel->s = "";
  this->sentinel->flink = snode;
  this->sentinel->blink = snode;            // create the sentinel node, then copy with Push_Back
  
  for ( node = d.Begin();
        node != d.End();
        node = node->Next() )
  {
    this->Push_Back(node->s);
  }
}

/* Assignment overload */

Dlist& Dlist::operator= (const Dlist &d)    // copies the content of existing objects (no new)
{
  Dnode *node;

  Clear();
   
  for ( node = d.Begin(); 
	    node != d.End(); 
	    node = node->Next() ) 
  {
    Push_Back(node->s);
  } 
  
  return *this;
}

/* Desctructor */

Dlist::~Dlist()
{
  delete this->sentinel;
  sentinel = NULL;
}

void Dlist::Clear()                         // should not delete the sentinel node
{
  while (!Empty()) (void) Erase(Begin());
}

/* Check if the list is empty */

bool Dlist::Empty() const
{
  return (size == 0);  
}

/* Return the size of the list */

size_t Dlist::Size() const
{
  return size;
}

/* Put new nodes/strings on the front or back of the list */

void Dlist::Push_Front(const string &s)
{
  this->Insert_Before(s, Begin());
}

void Dlist::Push_Back(const string &s)
{
  this->Insert_Before(s, End()); 
}

/* Remove and return the first and last element of the list */

string Dlist::Pop_Front()
{
  string fs;

  if (Empty()) return "";

  fs = Begin()->s;
  Erase(Begin());
  return fs;
}

string Dlist::Pop_Back()
{
  string bs;
  
  if (Empty()) return "";
  
  bs = Rbegin()->s;
  Erase(Rbegin());
  return bs;
}

//------------------------------------------------------------

/* Pointer to the first node on the list */

Dnode* Dlist::Begin() const
{
  return sentinel->flink;
}

/* Pointer to "one past" the last node on the list */

Dnode* Dlist::End() const
{
  return sentinel;
}

/* Pointer to the last node on the list */

Dnode* Dlist::Rbegin() const
{
  return sentinel->blink;
}

/* Pointer to "one before" the first node on the list */

Dnode* Dlist::Rend() const
{
  return sentinel;
}

Dnode* Dnode::Next()
{
  return this->flink;
}

Dnode* Dnode::Prev()
{
  return this->blink;
}

//------------------------------------------------------------

/* Insert a node before the current node */ 

void Dlist::Insert_Before(const string &s, Dnode *n)
{
  Dnode *prev;
  Dnode *newnode;
  
  newnode = new Dnode;
  newnode->s = s;

  prev = n->blink;

  newnode->blink = prev;
  newnode->flink = n;

  prev->flink = newnode;

  n->blink = newnode;

  size++;
}

/* Insert a node after the current node */

void Dlist::Insert_After(const string &s, Dnode *n)
{
  Insert_Before(s, n->Next());
}

/* Erase the current node */

void Dlist::Erase(Dnode *n)
{
  Dnode *prev;
  Dnode *next;
  
  if (n != sentinel) {
    prev = n->Prev();
    next = n->Next();

	prev->flink = next;
	next->blink = prev;

	delete n;  // Points to deleted memory
    n = NULL;  // makes sure you don't use it 

	size--;
  }
}	
