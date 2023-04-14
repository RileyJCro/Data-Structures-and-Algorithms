#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <map>

using namespace std;

/* I also wrote two procedures, which let me convert strings to times and back again. 
   These store the times in single integers (seconds).
   Write the main code that reads the lines of text. Convert each time to an integer and test. 
   
   Test with:  g++ -Wall -Wextra -std=c++98 -o step1 step1.cpp
               ./step_1 Rhapsody.txt
   */

class Song { 
  public:
    string title;
    int time;
    int track;
};

class Album {
  public:
    map <int, Song *> songs;
    string name;
    int time;
};

class Artist {
  public:
    map <string, Album *> albums;
    string name;
    int time;
    int nsongs;
};

/* A procedure that converts strings to times */

int Str_To_Time(string str) 
{
  int m, s;

  sscanf(str.c_str(), "%d:%d", &m, &s);

  m *= 60;

  return m + s;
}

/* A procedure that conversts times to strings */

string Time_To_Str(int total_sec) 
{
  char buf[2000];
  string str;
  int m, s;

  if (total_sec == 0) return ""; // What to do--

  m = total_sec / 60;

  s = total_sec % 60;

  sprintf(buf, "%d:%d", m, s);
  str = buf;

  return str;
}

int main(int argc, char *argv[])
{
  ifstream fin;
  string fileName;

  if (argc > 2) return -1;
  fileName = argv[1];

  fin.open(fileName.c_str());
  if (fin.fail()) return -1; // Check this

  istringstream ss;
  string line;
  int m, s;
  int total;

  string time, genre, track;

  Song song;
  Artist artist;

  while (getline(fin, line)) { 
    ss.clear();
	ss.str(line);

	// First 'word' is the title
	ss >> song.title;

	// Second 'word' is the time
	ss >> time;
    sscanf(time.c_str(), "%d:%d", &m, &s);
    
	// Third 'word' is the artist----
	ss >> artist.name;

	// Fourth 'word' is the genre---
	ss >> genre;

	// Last 'word' is the track---
	ss >> track;
	
	total = m * 60 + s;

	// sprintf(buf, "%d:%d", m, s);
	// str = buf;

	// total = Str_To_Time(line);
	// str = Time_To_Str(total);
	cout << time << " = " << total << " = " << time << endl;
  }
  
  fin.close();

  return 0;
}
