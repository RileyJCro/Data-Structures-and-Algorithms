#include "sorting.hpp"
#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

/* merge_1_sort.cpp
   Riley Crockett
   March 28, 2022

   This program implements merge sort on a vector. The sort_double() function
   is called in the sort_driver_lab.cpp program, and is what initially calls 
   the recursive merge sort function that is directly below.  
   */

void recursive_sort(vector <double> &v, vector <double> &temp, int start, int size, int print)
{
  int i, j, index;    // Two iterators for vector v, and one for the temp vector
  double tmp;         // A double for sorting the original vector in place

  if (size <= 1) return;    // Base case 1

  if (print) {
    printf("B: %5d %5d  ", start, size);
    for (i = 0; i < (int)v.size(); i++) printf("%5.2lf", v[i]);
    cout << endl;
  }
  
  if (size == 2) {                  // Base case 2, sort the vector in place
	if (v[start] > v[start+1]) {
	  tmp = v[start];
	  v[start] = v[start+1];
	  v[start+1] = tmp;
	}
  } else {    /* Otherwise split the vector recursively */

    recursive_sort(v, temp, start, size/2, print);    // Call on the left side of the vector

	/* Call on the right side of the vector. If the size is even both sides 
	   have equal size, otherwise the right split's size is 1 greater. */

    if (size % 2 == 0) {
	  recursive_sort(v, temp, start+size/2, size/2, print);
	} else {
	  recursive_sort(v, temp, start+size/2, size/2+1, print);
    }
	
	i = start;           // Set i to the start of the lower side
	j = start+size/2;    // Set j to the start of the upper side
	index = start;       // Set the initial index for the section to sort

	/* Loop through the two sides until one side has been traversed */

	while (i < start+size/2 && j < start+size) {
	  if (v[i] <= v[j]) {      // If the left value is <= to the right value
		temp[index] = v[i];    // Set the temp value at the index to the left value
		i++;                   // Increment the left iterator
	  } 
	  else if (v[i] >= v[j]) {    // If the left value is >= to the right value
        temp[index] = v[j];       // Set the temp value at the index to the right value
		j++;                      // Increment the left iterator
	  }
      index++;    // Increment the temp vector index for every value stored  
	}

	/* At this point one of the sides has been traversed. Finish 
	   traversing the remaining side, storing its values in temp. */

	while (i < start+size/2) {
	  temp[index] = v[i];
	  index++;
	  i++;
	}
	
	while (j < start+size) {
      temp[index] = v[j];
	  index++;
	  j++;
    }

	/* Copy the temporary vector values back to the original */

	for (i = 0; i < size; i++) v[start+i] = temp[start+i];
  }

  if (print) {
    printf("E: %5d %5d  ", start, size);
    for (i = 0; i < (int)v.size(); i++) printf("%5.2lf", v[i]);
    cout << endl;
  }

  return;
}

void sort_doubles(vector <double> &v, bool print)
{
  int i, n;    // An iterator and int for vector size
  
  n = v.size();                // Set n to the vector size 
  vector <double> temp (n);    // Initialize a temp vector with the same size
  
  recursive_sort(v, temp, 0, n, print);    // Call the recursive merge sort

  if (print) {
    printf("%16s", " ");
	for (i = 0; i < n; i++) printf("%5.2lf", v[i]);
    cout << endl;
  }

  return;
}
