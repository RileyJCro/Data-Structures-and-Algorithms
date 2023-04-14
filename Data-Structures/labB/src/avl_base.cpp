#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "avltree.hpp"
using namespace std;
using CS202::AVLTree;
using CS202::AVLNode;


AVLTree& AVLTree::operator= (const AVLTree &t)        
{
  (void) t;
  return *this;
}

/* I simply took Insert and Delete from their binary search tree
   implementations.  They aren't correct for AVL trees, but they are
   good starting points.  */

bool AVLTree::Insert(const string &key, void *val)
{
  AVLNode *parent;
  AVLNode *n;

  parent = sentinel;
  n = sentinel->right;

  /* Find where the key should go.  If you find the key, return false. */

  while (n != sentinel) {
    if (n->key == key) return false;
    parent = n;
    n = (key < n->key) ? n->left : n->right;
  }

  /* At this point, parent is the node that will be the parent of the new node.
     Create the new node, and hook it in. */

  n = new AVLNode;
  n->key = key;
  n->val = val;
  n->parent = parent;
  n->height = 1;
  n->left = sentinel;
  n->right = sentinel;

  /* Use the correct pointer in the parent to point to the new node. */

  if (parent == sentinel) {
    sentinel->right = n;
  } else if (key < parent->key) {
    parent->left = n;
  } else {
    parent->right = n;
  }

  /* Increment the size */

  size++;
  return true;
}
    
bool AVLTree::Delete(const string &key)
{
  AVLNode *n, *parent, *mlc;
  string tmpkey;
  void *tmpval;

  /* Try to find the key -- if you can't return false. */

  n = sentinel->right;
  while (n != sentinel && key != n->key) {
    n = (key < n->key) ? n->left : n->right;
  }
  if (n == sentinel) return false;

  /* We go through the three cases for deletion, although it's a little
     different from the canonical explanation. */

  parent = n->parent;

  /* Case 1 - I have no left child.  Replace me with my right child.
     Note that this handles the case of having no children, too. */

  if (n->left == sentinel) {
    if (n == parent->left) {
      parent->left = n->right;
    } else {
      parent->right = n->right;
    }
    if (n->right != sentinel) n->right->parent = parent;
    delete n;
    size--;

  /* Case 2 - I have no right child.  Replace me with my left child. */

  } else if (n->right == sentinel) {
    if (n == parent->left) {
      parent->left = n->left;
    } else {
      parent->right = n->left;
    }
    n->left->parent = parent;
    delete n;
    size--;

  /* If I have two children, then find the node "before" me in the tree.
     That node will have no right child, so I can recursively delete it.
     When I'm done, I'll replace the key and val of n with the key and
     val of the deleted node.  You'll note that the recursive call 
     updates the size, so you don't have to do it here. */

  } else {
    for (mlc = n->left; mlc->right != sentinel; mlc = mlc->right) ;
    tmpkey = mlc->key;
    tmpval = mlc->val;
    Delete(tmpkey);
    n->key = tmpkey;
    n->val = tmpval;
    return true;
  }

  return true;
}
               
vector <string> AVLTree::Ordered_Keys() const
{
  /* Create a string vector to hold keys, call the recursive function
     on the root of the tree and return the keys in sorted order. */

  vector <string> rv;
  make_key_vector(sentinel->right, rv);
  return rv;
}
    
void AVLTree::make_key_vector(const AVLNode *n, vector<string> &v) const
{
  if (n == sentinel) return;       // Exit after a leaf node or on an empty tree
  make_key_vector(n->left, v);     // Iterate through the lesser/left nodes first
  v.push_back(n->key);             // Push back the keys to the vector
  make_key_vector(n->right, v);    // Iterate through the greater/right nodes
}
     
size_t AVLTree::Height() const  // need to update this--------------
{
  int lDepth, rDepth;    // Integers for left & right tree depth
  int height;            // An integer for greatest tree depth

  if (n == sentinel) return 0;                 // Exit on empty tree
  lDepth = recursive_find_height(n->left);     // Iterate through left trees
  rDepth = recursive_find_height(n->right);    // Iterate through right trees

  height = (lDepth < rDepth) ? rDepth : lDepth;    // Set height to the greatest depth
  return height + 1;
  // return 0;
}

/* You need to write this to help you with the assignment overload.
   It makes a copy of the subtree rooted by n.  That subtree is part
   of a different tree -- the copy will be part of the tree that
   is calling the method. */

AVLNode *AVLTree::recursive_postorder_copy(const AVLNode *n) const
{
  (void) n;
  return NULL;
}
