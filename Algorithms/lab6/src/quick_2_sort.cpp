#include "sorting.hpp"
#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

/* quick_sort_2.cpp
   Riley Crockett
   March 28, 2022

   This program implements quicksort #2 on a vector. It differs from quicksort #1 
   by setting the pivot to the median of the start, middle, and end indices of the partition. 
   The sort_double() function is called in the sort_driver_lab.cpp program, and is what 
   initially calls the quicksort function that is directly below.  
   */

void recursive_sort(vector <double> &v, int start, int size, int print)
{
  int i;                  // An iterator
  int lp, rp;             // Left and right iterators for performing the partition
  int mid, end, pivot;    // Integers to store the middle, end, and pivot indices
  double tmp;             // A temporary double to swap vector values

  if (size <= 1) return;     // Base case 1

  if (print) {
    printf("S: %5d %5d %5s", start, size, " ");
    for (i = 0; i < (int)v.size(); i++) printf("%5.2lf", v[i]);
    cout << endl;
  }

  if (size == 2) {                  // Base case 2, sort the vector in place
    if (v[start] > v[start+1]) {
      tmp = v[start];
      v[start] = v[start+1];
      v[start+1] = tmp;
    }
	return;
  }      
	
  mid = start+size/2;
  end = start+size-1;    
  pivot = start;         // The pivot is initially set to the start

  // If mid and end the same but not start, the pivot is end
  if (v[mid] == v[end] && v[start] != v[end]) { 
    pivot = end;
  }  // If mid is the median, the pivot is mid
  else if ((v[start] < v[mid] && v[mid] < v[end]) || (v[end] < v[mid] && v[mid] < v[start])) {
    pivot = mid;
  }  // If end is the median, the pivot is end
  else if ((v[start] < v[end] && v[end] < v[mid]) || (v[mid] < v[end] && v[end] < v[start])) {
    pivot = end;
  }
	
  tmp = v[pivot];         // Swap the values at the pivot and start
  v[pivot] = v[start];
  v[start] = tmp;
  pivot = start;

  if (print) {
    printf("M: %5d %5d %5.2lf", start, size, v[pivot]);
    for (i = 0; i < (int)v.size(); i++) printf("%5.2lf", v[i]);
    cout << endl;
  }

  lp = start+1;    // Set the left and right pointers
  rp = end;
    
  while (1) {
    /* Increment the left pointer until finding an element >= the pivot, or until
	   going out of scope. Then, decrement the right pointer until finding an 
	   element <= the pivot, or until indexing the start. */

	while (lp < end+1 && v[lp] < v[pivot]) lp++;
	while (rp > start && v[rp] > v[pivot]) rp--;
      
    /* If the pointers have not crossed, swap their vector values, increment the left 
	   pointer, decrement the right pointer, and continue. Otherwise the loop breaks. */

	if (lp < rp) {
      tmp = v[lp];
	  v[lp] = v[rp];
	  v[rp] = tmp;
	  lp++;
	  rp--;
	  continue;
	}

	break;
  }
    
  if (lp == rp) lp++;    // If the left and right pointers are equal, increment the left

  tmp = v[pivot];        // Swap the pivot with the last element of the left set
  v[pivot] = v[lp-1];
  v[lp-1] = tmp;
  pivot = lp-1;

  if (print) {
    printf("P: %5d %5d %5d", start, size, pivot);
    for (i = 0; i < (int)v.size(); i++) printf("%5.2lf", v[i]);
    cout << endl;
  }

  /* Recursively sort the left and right sets */

  recursive_sort(v, start, pivot-start, print);
  recursive_sort(v, pivot+1, end-pivot, print);	

  return;
}

void sort_doubles(vector <double> &v, bool print)
{
  int i, n;    // An iterator and int for vector size

  n = v.size();

  recursive_sort(v, 0, n, print);    // Call the recursive quicksort
	
  if (print) {
    printf("%20s", " ");
    for (i = 0; i < n; i++) printf("%5.2lf", v[i]);
    cout << endl;
  }

  return;
}
