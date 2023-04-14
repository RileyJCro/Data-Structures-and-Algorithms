#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS202::BSTree;
using CS202::BSTNode;

/* ---------------------------------------------- */
/* You'll write all of these as part of your lab. */

int BSTree::Depth(const string &key) const
{
  (void) key;
  return -1;
}
           
int BSTree::Height() const
{
  return -1;
}

vector <string> BSTree::Ordered_Keys() const
{
  vector <string> rv;
  return rv;
}
    
BSTree::BSTree(const BSTree &t)        
{
  (void) t;
}

BSTree& BSTree::operator= (const BSTree &t) 
{
  (void) t;
  return *this;
}

int BSTree::recursive_find_height(const BSTNode *n) const
{
  (void) n;
  return -1;
}

void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const
{
  (void) n;
  (void) v;
}

BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, 
                            const vector<void *> &vals,
                            size_t first_index,
                            size_t num_indices) const
{
  (void) sorted_keys;
  (void) vals;
  (void) first_index;
  (void) num_indices;
  return NULL;
}
