#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <map>

using namespace std;

/* Deal with the artists. Have an artist map to hold the artists. When reading in a song,
     check for the artist in the map. If it's there print "Old Artist" otherwise create the 
	 artist, set the name and put it into the map and print "New Artist" and the name.

   For testing:    g++ -Wall -Wextra -std=c++98 -o step3 step3.cpp
                   ./step3 Rhapsody.txt
				   ./step3 Dreaming.txt
				   ./step3 Lark.txt
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

int Str_To_Time(string str) 
{
  int m, s;
  sscanf(str.c_str(), "%d:%d", &m, &s);
  m *= 60;
  return m + s;
}

string Time_To_Str(int total_sec) 
{
  char buf[2000];
  string str;
  int m, s;
  if (total_sec == 0) return "";
  m = total_sec / 60;
  s = total_sec % 60;
  sprintf(buf, "%d:%d", m, s);
  str = buf;
  return str;
}

/* A procedure that changes underscores to spaces */

void UnderscoreToSpace(string &str)  // Pass-by reference?
{
  for (size_t i = 0; i < str.size(); i++) {
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
  if (fin.fail()) return -1;

  istringstream ss;
  string line;
  string time, track, genre;
  
  map <string,Artist> artists; // A map to hold the artists
  map <string,Artist>::iterator it;

  Artist artist;
  Album album;

  while (getline(fin, line)) { 
    ss.clear();
	ss.str(line);

    Song song;
    Artist artist;

	ss >> song.title;               // First 'word' is the title
	UnderscoreToSpace(song.title);  // Replace underscores with spaces

	ss >> time;                     // Second 'word' is the time
	song.time = Str_To_Time(time);  // Convert the string format to an integer

	ss >> artist.name;  // Third 'word' is the artist name

	ss >> album.name;   // Fourth 'word' is the album name
	
	ss >> genre;        // Fifth 'word' is the genre
	
	ss >> song.track;   // Sixth 'word' is the track number

	it = artists.find(artist.name);
    
	// If there is a new artist
	if (it == artists.end()) {
	  artists.insert(pair<string,Artist>(artist.name,artist));
      cout << "New Artist: ";
	} else {
	  cout << "Old Artist: ";
	}

    cout << artist.name << endl;
    
	// cout << song.title << " " << song.track << " " << song.time << endl;
  }
  
  fin.close();

  return 0;
}
