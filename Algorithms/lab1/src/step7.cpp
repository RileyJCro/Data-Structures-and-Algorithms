#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <map>

using namespace std;

/* Get the comments from commented6.cpp */

class Song { 
  public:
    string title;
    int time;
    int track;
};

class Album {
  public:
    map <int, Song> songs;
    string name;
    int time;
};

class Artist {
  public:
    map <string, Album> albums;
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
  sprintf(buf, "%d:%02d", m, s);
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
  istringstream ss;
  string fileName, line;
  string time, genre;

  map <string,Artist> artists;           
  map <string,Artist>::iterator art_it;
  map <string,Album>::iterator alb_it;
  map <int, Song>::iterator song_it;

  if (argc > 2) return -1;
  fileName = argv[1]; fin.open(fileName.c_str());
  if (fin.fail()) return -1;

  while (getline(fin, line)) { 
    ss.clear();
	ss.str(line);
    
	Song song;
    Artist artist;
    Album album;
	
	album.time = 0;    // Initialize the album's time to '0'
	
	ss >> song.title;               // First 'word' is the title
	UnderscoreToSpace(song.title);  // Replace underscores with spaces
	
	ss >> time;                     // Second 'word' is the time
	song.time = Str_To_Time(time);  // Convert the string format to an integer
	
	ss >> artist.name;              // Third 'word' is the artist name
	UnderscoreToSpace(artist.name);

	ss >> album.name;               // Fourth 'word' is the album name
	UnderscoreToSpace(album.name);

	ss >> genre;        // Fifth 'word' is the genre
	ss >> song.track;   // Sixth 'word' is the track #

	art_it = artists.find(artist.name);

	if (art_it == artists.end()) {
	  artists.insert(pair<string,Artist>(artist.name,artist));
	  art_it = artists.find(artist.name); // Just find it again?
	} 
	
	alb_it = art_it->second.albums.find(album.name);

	if (alb_it == art_it->second.albums.end()) {
	  art_it->second.albums.insert(pair<string,Album>(album.name,album));
	  alb_it = art_it->second.albums.find(album.name);
	}
    
    alb_it->second.songs.insert(pair<int,Song>(song.track,song));

	art_it->second.time += song.time;  // Update the artist's time
	alb_it->second.time += song.time;  // Update the album's time
	art_it->second.nsongs++;           // Update the artist's number songs
  }

  fin.close();
  
  for (art_it = artists.begin(); art_it != artists.end(); art_it++) {
     cout << art_it->first << ": " << art_it->second.nsongs 
	  	  << ", " << Time_To_Str(art_it->second.time) << endl;
    
	for (alb_it = art_it->second.albums.begin(); alb_it != art_it->second.albums.end(); alb_it++) {
	  cout << "        " << alb_it->first << ": " << alb_it->second.songs.size()
	       << ": " << Time_To_Str(alb_it->second.time) << endl;
      
	  for (song_it = alb_it->second.songs.begin(); song_it != alb_it->second.songs.end(); song_it++){
        cout << "                " << song_it->first << ". " << song_it->second.title
		     << ": " << Time_To_Str(song_it->second.time) << endl;
      }
    }
  }

  return 0;
}
