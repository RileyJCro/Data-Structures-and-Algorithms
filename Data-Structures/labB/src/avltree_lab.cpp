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

/* avltree_lab.cpp
   Riley Crockett
   December 2, 2021

   This program implements methods of the avltree.hpp header file.
   The Insert and Delete methods are defined at the bottom of the program and are
   only slightly modified from the binary search tree implementation. The changes are
   denoted by comments with ------x in them.

   Currently, the assignment overload is not working properly and fails the last half 
   of the gradescripts. It supports the PRINT_COPY command, but not ASSIGNMENT. 
   */

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

void fix_height(AVLNode *n)
{
  if (n->height == 0) return;

  /* Set the current node's height to the greater subtree's height + 1 */

  if (n->right->height < n->left->height) {
    n->height = n->left->height + 1;
  } else {
    n->height = n->right->height + 1;
  }
}

AVLTree& AVLTree::operator= (const AVLTree &t)        
{
  /* Create a sentinel node for the tree */

  AVLNode *sn = new AVLNode;
  this->sentinel = sn;
  
  /* Call the recursive function on the root of the tree */

  sn->right = recursive_postorder_copy(t.sentinel->right);
  this->sentinel->right = sn->right;

  return *this;
}

size_t AVLTree::Height() const
{
  return sentinel->right->height;    // Return the height of the root node
}

AVLNode *AVLTree::recursive_postorder_copy(const AVLNode *n) const
{
  if (n->height == 0) return sentinel;
  
  AVLNode *root = new AVLNode;

  root->left = recursive_postorder_copy(n->left);
  root->right = recursive_postorder_copy(n->right);

  root->key = n->key;
  root->val = n->val;
  root->height = n->height;

  root->left->parent = root;
  root->right->parent = root;
  
  return root;
}

bool imbalance(const AVLNode *n)
{
  int l_height = n->left->height;     // An integer for left subtree height
  int r_height = n->right->height;    // An integer for right subtree height

  /* Check the heights of the node's children. If the height difference
     is greater than 1, return true. Otherwise return false. */
  
  if (l_height - r_height > 1 || r_height - l_height > 1) return true;

  return false;
}

void rotate(AVLNode *n)
{
  AVLNode *middle;                          // A pointer to the node that switches
  AVLNode *parent = n->parent;              // A pointer to the rotation node's parent
  AVLNode *grandparent = parent->parent;    // A pointer to the rotation node's grandparent
  
  if (parent->height == 0) return;    // Exit if the parent is the sentinel node

  /* Set the middle node based on the keys of n and its parent */

  if (parent->key < n->key) {
	middle = n->left;
    n->left = parent;          // Set the parent to the left child of n
	parent->right = middle;    // The middle node becomes the right child of the old parent
  } else {
	middle = n->right;
    n->right = parent;         // Set the parent to the right child of n
	parent->left = middle;     // The middle node becomes the left child of the old parent
  }

  n->parent = grandparent;
  middle->parent = parent; 
  parent->parent = n;

  /* Set n to be a child of the grandparent based on their keys */

  if (grandparent->key < n->key) {
	grandparent->right = n;
  } else {
    grandparent->left = n;
  }
  
  /* Fix the heights after the rotation is complete */ 

  fix_height(n->left);
  fix_height(n->right);
  fix_height(n);
  fix_height(n->parent); 
}

void fix_imbalance(AVLNode *n)
{
  AVLNode *grandchild;    // A node pointer to the grandchild

  /* Determine the type of imbalance case */

  if (n->left->height > n->right->height) {	
    if (n->left->left->height > n->left->right->height || 
		n->left->left->height == n->left->right->height) {
	  /* Left Zig-Zig (rotate about the left child) */
	  rotate(n->left);
    } 
	else {
	  /* Left Right Zig-Zag (rotate about the grandchild twice) */
	  grandchild = n->left->right;
      rotate(grandchild);
      rotate(n->left);
    }
  } else {
    if (n->right->left->height < n->right->right->height ||
		n->right->left->height == n->right->right->height) {
	  /* Right Zig-Zig (rotate about the right child) */
	  rotate(n->right);
    } 
	else {
	  /* Right Left Zig-Zag (rotate about the grandchild twice) */
      grandchild = n->right->left;
      rotate(grandchild);
      rotate(n->right);
    }
  }
}

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

  /* Traverse from the inserted node to the root. Update node  ------x
     heights and fix any imbalances. */
 
  n = n->parent;
  while (n != sentinel) {
	fix_height(n);
    if (imbalance(n)) { 
	  fix_imbalance(n); 
	}
	n = n->parent;
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

  /* Traverse from the deleted node to the root. Update node  ------x
     heights and fix any imbalances */

  n = n->parent;
  while (n != sentinel) {
    fix_height(n);
    if (imbalance(n)) {
      fix_imbalance(n);
    }
    n = n->parent;
  }

  return true;
}
