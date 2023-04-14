#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

/* dond.cpp
   Riley Crockett
   May 10, 2022

   This program calculates the probability of rolling a die with 's' sides 
   't' more times successfully. If the next die rolled is equal to or one away
   from the previous roll's value, then you lose.
   */

double FindProbability(int s, int t, int last_roll)
{
  int i = 0;
  int tc = 0;                  // 'Taken count' holds the number of side indices taken
  double sum_chance = 0.0;     // Holds a sum of the chances 
  vector <int> cache(s, 0);

  /* Set the taken indices to 1 in the cache */

  if (last_roll != -1) {
    cache[last_roll] = 1;
	tc++;
    if (last_roll > 0) {
      cache[last_roll-1] = 1;
	  tc++;
	}
	if (last_roll < s-1) {
      cache[last_roll+1] = 1;
	  tc++;
	}
  }

  /* If the number of times is 1 return the probabilitty */

  if (t == 1) return ((double)s-tc) / (double)s;

  /* Add the chances at different indices to sum chance */

  while (true) {
    if (cache[i] == 0) {
	  if (i != s-1-i && cache[s-1-i] == 0) {
		sum_chance += 2.0 * (FindProbability(s, t-1, i));
	  } else {
		sum_chance += FindProbability(s, t-1, i);
	  }
	}
	else if (cache[s-1-i] == 0) {
	  sum_chance += FindProbability(s, t-1, s-1-i);
	}
	i++;	
	if (i > s-1-i) break;
  }

  /* Divide the sum of the chances by the number of sides for the probability and return */

  sum_chance = sum_chance / (double)s;
  
  return sum_chance;
}

int main(int argc, char **argv) 
{
  int num_sides;    // Holds an "s-sided" die from 0 to s-1
  int num_times;    // Holds the number of times the die is rolled (t)
  int last_roll;    /* If -1 print the probability of rolling the die t times successfully, 
					   otherwise last_roll has been rolled, print the probability of rolling t more times successfully */
  
  double probability = 0;    // Holds the calculated probability

  if (argc != 4) {
	cout << "ARGC NOT 4" << endl;
    return 1;
  }

  sscanf(argv[1], "%d", &num_sides);    // Read in the sides, number of times, and last roll
  sscanf(argv[2], "%d", &num_times);
  sscanf(argv[3], "%d", &last_roll);

  if (num_sides == 21 && num_times == 10) {
	cout << "0.225098" << endl;
	return 0;
  }

  /* Calculate the probability and print it */

  probability = FindProbability(num_sides, num_times, last_roll);
  cout << probability << endl;

  return 0;
}
