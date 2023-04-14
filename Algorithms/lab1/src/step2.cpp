#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <map>

using namespace std;

/* Convert all the underscores to spaces when you read words in. 
   Then, create the song class instance when you read a song, and print it 
   
   For testing:    g++ -Wall -Wextra -std=c++98 -o step2 step2.cpp
                   ./step2 Rhapsody.txt
				   ./step2 Dreaming.txt
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

//--------------------------------------------------------

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

/* A procedure that changes underscores to spaces */
void UnderscoreToSpace(string &str)  // Pass-by reference?
{
  size_t i;

  for (i = 0; i < str.size(); i++) {
    if (str[i] == '_') str[i] = ' ';
  }
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
  // int total;
  string time, track, genre;
  
  Artist artist;
  Album album;

  while (getline(fin, line)) { 
    ss.clear();
	ss.str(line);

    Song song;

	// First 'word' is the title
	ss >> song.title;
	UnderscoreToSpace(song.title);  // Replace underscores with spaces

	// Second 'word' is the time
	ss >> time;
    sscanf(time.c_str(), "%d:%d", &m, &s);
	// song.time = time;
    
	// Third 'word' is the artist----
	ss >> artist.name;

	// Fourth 'word' is the album name
	ss >> album.name;
	
	// Fifth 'word' is the genre
	ss >> genre;
	
	// Sixth 'word' is the track number
	ss >> song.track;
	// sscanf(track.c_str(), "%d", &song.track);
	

	// total = m * 60 + s;

	cout << song.title << " " << song.track << " " << time << endl;
  }
  
  fin.close();

  return 0;
}
