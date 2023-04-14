#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS202::BSTree;
using CS202::BSTNode;

/* bstree_lab.cpp
   Riley Crockett
   November 23, 2021

   This program implements some of the methods of the binary search tree API
   defined in the bstree.hpp header file. The methods are as follows:
   -Depth (returns the distance of a node from the root, indexed by its key)
   -Height (returns the greatest depth of a node in the tree, has a recursive helper)
   -Ordered_Keys (returns a vector of node keys in sorted order, has a recursive helper)
   -Copy constructor and assignment overload
   -make_balanced_tree (assignment overload helper, returns roots of a balanced tree)
   */

int BSTree::Depth(const string &key) const
{
  BSTNode *n;      // A tree node pointer
  int depth;       // An int to store node depth

  /* Exit if the node is the sentinel (key not found),
     or the key matches (return depth). Otherwise, point
	 to the next node (left if the key is less than, right
	 if the key is greater than), and increment the depth. */

  n = sentinel->right;      // Point to the first node

  while (1) {
    if (n == sentinel) return -1;
	if (key == n->key) return depth;
    n = (key < n->key) ? n->left : n->right;
	++depth;
  }
}
           
int BSTree::Height() const
{
  /* Call the recursive function on the first node */

  return recursive_find_height(sentinel->right);
}

vector <string> BSTree::Ordered_Keys() const
{
  /* Create a string vector to hold keys, call the recursive function 
     on the root of the tree, and return the keys in sorted order. */

  vector <string> rv;
  make_key_vector(sentinel->right, rv);
  return rv;
}

/* The copy constructor */

BSTree::BSTree(const BSTree &t)        
{
  *this = t;    
}

/* The assignment overload */

BSTree& BSTree::operator= (const BSTree &t) 
{
  vector <string> sorted_keys = t.Ordered_Keys();    // Stores keys in sorted order 
  vector <void *> vals = t.Ordered_Vals();           // Stores vals in sorted order
  
  /* Create a sentinel node for the tree */

  BSTNode *sn = new BSTNode;
  this->sentinel = sn;
  
  /* Call the recursive function and return the root of the balanced tree */

  sn->right = make_balanced_tree(sorted_keys, vals, 0, vals.size());   
  this->sentinel->right = sn->right;
  this->size = vals.size();
  
  return *this;
}

int BSTree::recursive_find_height(const BSTNode *n) const
{
  int lDepth, rDepth;    // Integers for left & right tree depth
  int height;            // An integer for greatest tree depth
  
  if (n == sentinel) return 0;                 // Exit on empty tree 
  lDepth = recursive_find_height(n->left);     // Iterate through left trees
  rDepth = recursive_find_height(n->right);    // Iterate through right trees
  
  height = (lDepth < rDepth) ? rDepth : lDepth;    // Set height to the greatest depth
  return height + 1;                               // Return the height plus one
}

void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const
{
  if (n == sentinel) return;       // Exit after a leaf node or on an empty tree
  make_key_vector(n->left, v);     // Iterate through the lesser/left nodes first
  v.push_back(n->key);             // Push back the keys to the vector
  make_key_vector(n->right, v);    // Iterate through the greater/right nodes
}

BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, 
                            const vector<void *> &vals,
                            size_t first_index,
                            size_t num_indices) const
{
  /* First base case */

  if (num_indices == 0) return sentinel;

  BSTNode *root = new BSTNode;                     // A new node to hold a root
  size_t mid = first_index + (num_indices / 2);    // Store the middle index of a subtree

  /* Set the root's key and val to the subtree's middle index */

  root->key = sorted_keys[mid];
  root->val = vals[mid];

  /* Second base case: On a leaf node, left and right point to the sentinel */

  if (num_indices == 1) {
	root->left = sentinel;
	root->right = sentinel;
	return root; 
  } 
  
  /* Iterate through the left subtrees */

  root->left = make_balanced_tree(sorted_keys, vals, first_index, num_indices / 2);
  
  /* Iterate through the right subtrees. If there is an even number of indices,
     the right subtree will have 1 less than half of the indices. 
	 Otherwise it will have half of the indices */

  if (num_indices % 2 == 0) {
    root->right = make_balanced_tree(sorted_keys, vals, mid + 1, num_indices / 2 - 1);
  } else {
	root->right = make_balanced_tree(sorted_keys, vals, mid + 1, num_indices / 2);
  }
  
  /* Set the childrens' parent to the root */

  root->left->parent = root;
  root->right->parent = root;

  return root;
}
