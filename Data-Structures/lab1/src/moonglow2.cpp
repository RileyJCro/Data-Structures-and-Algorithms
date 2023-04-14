#include <iostream>
using namespace std;
/*
	
	*/
int main()
{
	string s, name;
	double num = 0, score = 0, sum = 0, ac = 0;
	

	while (!cin.eof()) {

		if (cin >> num) {
			score += num;
		} else {
			cin.clear();
			cin >> s;

			if (s == "NAME") {
				cin >> name;
			} 
			else if (s == "AVERAGE") {
				while (cin >> num) {
					sum += num;
					ac++;
				}
				
				if (ac != 0) {
					score += sum / ac;
				}

				cin.clear();
				sum = 0;
				ac = 0;
			}
		}
	}

	cout << name << " " << score << endl;

	return 0;
}

