#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <map>

using namespace std;

/* Insert the songs into the albums' song lists. Traverse the artists/albums 
   after reading from the file. Just print out the names, not the songs or times.

   For testing:    g++ -Wall -Wextra -std=c++98 -o step5 step5.cpp
				   ./step5 Dreaming.txt
				   ./step5 Rhapsody.txt
				   ./step5 Lark.txt
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
  
  map <string,Artist> artists;           // A map to hold the artists
  map <string,Artist>::iterator art_it;
  map <string,Album *>::iterator alb_it; // An iterator for an artist's album


  while (getline(fin, line)) { 
    ss.clear();
	ss.str(line);
    
	Song song;
    Artist artist;
    Album album;
	
	ss >> song.title;               // First 'word' is the title
	UnderscoreToSpace(song.title);  // Replace underscores with spaces
	
	ss >> time;                     // Second 'word' is the time
	song.time = Str_To_Time(time);  // Convert the string format to an integer
	
	ss >> artist.name;  // Third 'word' is the artist name
	UnderscoreToSpace(artist.name);

	ss >> album.name;   // Fourth 'word' is the album name
	UnderscoreToSpace(album.name);

	ss >> genre;        // Fifth 'word' is the genre
	ss >> song.track;   // Sixth 'word' is the track number

	album.songs.insert(pair<int,Song *>(song.track,&song));

	art_it = artists.find(artist.name);

	// If there is a new artist
	if (art_it == artists.end()) 
	{
	  artists.insert(pair<string,Artist>(artist.name,artist));
	  art_it = artists.find(artist.name); // Just find it again?
	} 
	
	alb_it = art_it->second.albums.find(album.name);

	// If there is a new album
	if (alb_it == art_it->second.albums.end())
	{
	  art_it->second.albums.insert(pair<string,Album *>(album.name,&album)); 
	}

  }

  fin.close();

  // After reading from file traverse artists/albums
  // (just print out the artist and their album names (indented to 8 spaces))
  
  for (art_it = artists.begin(); art_it != artists.end(); art_it++) {
    cout << art_it->first << endl;
    
	for (alb_it = art_it->second.albums.begin(); alb_it != art_it->second.albums.end(); alb_it++) {
    
	  cout << "        " << alb_it->first << endl;
    }
  }

  return 0;
}
