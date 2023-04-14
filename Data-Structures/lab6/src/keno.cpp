#include <iostream>
#include <cstdio>
#include <vector>
#include "fraction.hpp"
using namespace std;

/*  keno.cpp
    Riley Crockett
	October 19, 2021

	This program uses the Fraction class and its methods to calculate the 
	odds/probability of a game called Keno. 
	It reads a bet amount, number of balls picked, and pairs of matches with
	their respective payouts, on standard input. It then calculates the 
	probabilities and returns of matches, and prints them.
    */

int main() 
{
  size_t i;                      // The loop iterator
  double bet;                    // The amount to bet
  int picked;                    // The number of balls picked
  int match;                     // The number of balls to match
  double payout;                 // The payout associated with a certain match
  vector <int> match_vec;        // The vector to store matches
  vector <double> payout_vec;    // The vector to store payouts of a match 
  Fraction fract;                // The instance of the Fraction class
  double probability;            // The probability of a match
  double expRet;                 // The expected return of a specific match
  double retPerBet;              // The return per bet

  /* Read-in/store the bet, and number of balls picked */

  cin >> bet >> picked; 

  /* Store every pair of matches with their respective payout in vectors */

  while (cin >> match >> payout) {
    match_vec.push_back(match);
	payout_vec.push_back(payout);
  }
  /* Print the bet, and number of balls picked */
 
  printf("Bet: %.2f\n", bet);
  cout << "Balls Picked: " << picked << endl;
  
  /* Loop for the number of match instances */

  for (i = 0; i < match_vec.size(); i++) {
	
	/* Create a fraction for the probability of a specific match. 
	   Perform: binom(80-p,20-c) * binom(p,c) / binom(80,20)   */

	fract.Multiply_Binom(80 - picked, 20 - match_vec[i]);
	fract.Multiply_Binom(picked, match_vec[i]);
	fract.Divide_Binom(80, 20);

    probability = fract.Calculate_Product();    // Store the probability
	expRet = probability * payout_vec[i];       // Calculate the expected return
	retPerBet += expRet;                        // Add expected return to return per bet
	fract.Clear();                              // Clear the fraction for next loop

	/* Print the probability of a specific match and its expected return */

	cout << "  Probability of catching " 
	  << match_vec[i] << " of " << picked << ": " << probability 
	  << " -- Expected return: " << expRet << endl;
  }

  retPerBet -= bet;    // Subtract the bet from return per bet

  /* Print the return per bet, and the normalized return */

  printf("Your return per bet: %.2f\n", retPerBet);
  printf("Normalized: %.2f\n", retPerBet / bet);
  return 0;
}
