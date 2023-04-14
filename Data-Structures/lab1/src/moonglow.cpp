#include <iostream>
using namespace std;

/*	moonglow.cpp
	Riley Crockett
	August 30, 2021

	Reads a file in Moonglow's format as standard input, and prints a condensed 
	version as output.

	Loops through the standard input, stores the name, calculates any averages,
	and updates the score.
	*/


int main()
{
	string s, name;		/* The input string and name */

	/* The input number, total score, sum of #'s in avg, and # of #'s in avg */

	double num = 0, score = 0, sum = 0, ac = 0;
	

	while (!cin.eof()) {

		/*	Attempt to read input as an int.
			A number by itself is added to the score.
			Otherwise clear cin and read input as a string. 
			*/

		if (cin >> num) {
			score += num;
		} else {
			cin.clear();
			cin >> s;
			
			/* Store next input in name if previous string was "NAME" */

			if (s == "NAME") {
				cin >> name;
			} 

			/* Calculate the average of #'s if previous string was "AVERAGE" */

			else if (s == "AVERAGE") {
				while (cin >> num) {
					sum += num;				/* Add current input to the sum */ 
					ac++;					/* Count how many #'s in avg */
				}
				
				/* Add the average to the score */

				if (ac != 0) {
					score += sum / ac;
				}
				
				/* Reset cin and variables used for the average */

				cin.clear();
				sum = 0;
				ac = 0;
			}
		}
	}
	
	/* Print file info as a simple output */

	cout << name << " " << score << endl;

	return 0;
}

