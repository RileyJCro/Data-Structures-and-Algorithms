#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <map>
using namespace std;
/* */

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
  char buf[20];
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
void UnderscoreToSpace(string &str)  // Pass-by reference? do it with scanf?
{
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] == '_') str[i] = ' ';
  }
}

int main(int argc, char *argv[])
{
  ifstream fin;
  string fileName;
  istringstream ss;
  string line, time, track, genre;

  map <string, Artist *> artists;              // A map to hold all of the artists
  map <string, Artist *>::iterator art_it;     // The artists map iterator
  map <string, Album *>::iterator alb_it;    // The albums map iterator
  map <int, Song *>::iterator song_it;       // The songs map iterator

  if (argc > 2) return -1;
  fileName = argv[1]; fin.open(fileName.c_str());
  if (fin.fail()) return -1;
  
  while (getline(fin, line)) { 
    ss.clear();
	ss.str(line);
    
	Artist artist;    // Create instances of the artist, album, and song
	Album album;
	Song song;    
	
	ss >> song.title;                 // First 'word' is the title
	UnderscoreToSpace(song.title);    // Replace underscores with spaces
	
	ss >> time;                       // Second 'word' is the time
	song.time = Str_To_Time(time);    // Convert the string format to an integer
	
	ss >> artist.name;                // Third 'word' is the artist name
	UnderscoreToSpace(artist.name);

	ss >> album.name;                 // Fourth 'word' is the album name
	UnderscoreToSpace(album.name);

	ss >> genre;        // Fifth 'word' is the genre
	ss >> song.track;   // Sixth 'word' is the track number

	// album.songs.insert(pair<int, Song *>(song.track, &song));  // Insert the song into the album (indexed on track#)

    /* Attempt to find the artist name in the artists map. 
	   If it's not in the map, insert the new artist. */

	art_it = artists.find(artist.name);

	if (art_it == artists.end()) { 
	  artists.insert(pair<string, Artist *>(artist.name, &artist));
	  art_it = artists.find(artist.name); // Just find it again?
	} 
	
    /* Attempt to find the album name in the artist's albums. 
	   It it's not there, insert the new album */

	alb_it = art_it->second->albums.find(album.name);

    // alb_it = artists[artist.name]->albums.find(album.name);

	if (alb_it == art_it->second->albums.end()) {
	  // artists[artist.name]->albums.insert(pair<string, Album *>(album.name, &album));
	  art_it->second->albums.insert(pair<string, Album *>(album.name, &album));
	  alb_it = art_it->second->albums.find(album.name);
	  // alb_it = artists[art_it->first]->albums.find(album.name);
	}

	/* Insert the song under the current artist and album */

	// art_it->second->albums[alb_it->first]->songs.insert(pair<int, Song *>(song.track, &song));
    art_it->second->albums[alb_it->first]->songs.insert(pair<int, Song *>(song.track, &song));
	// artists[artist.name]->albums[album.name]->songs.insert(pair<int, Song *>(song.track, &song));

	/*
	song_it = art_it->second->albums[alb_it->first]->songs.find(song.track);

	cout << art_it->first << endl;
	cout << "        " << alb_it->first << endl;
	cout << "                " << song_it->second->track << ". " << song_it->second->title
         << ": " << Time_To_Str(song_it->second->time) << endl;

	*/
    /*	
	for (art_it = artists.begin(); art_it != artists.end(); art_it++) {
      cout << art_it->first << endl;

      for (alb_it = artists[art_it->first]->albums.begin(); alb_it != artists[art_it->first]->albums.end(); alb_it++) {
        cout << "        " << alb_it->first << endl;

        for (song_it = artists[art_it->first]->albums[alb_it->first]->songs.begin();
             song_it != artists[art_it->first]->albums[alb_it->first]->songs.end(); song_it++) {

          cout << "                " << song_it->second->track << ". " << song_it->second->title
             << ": " << Time_To_Str(song_it->second->time) << endl;
        }
      }
    }
    */
  }
  
  fin.close();    // Close the file after reading

  for (art_it = artists.begin(); art_it != artists.end(); art_it++) {
    cout << art_it->first << endl;

    for (alb_it = art_it->second->albums.begin(); alb_it != art_it->second->albums.end(); alb_it++) {
      cout << "        " << alb_it->first << endl;
      /*
      for (song_it = art_it->second->albums[alb_it->first]->songs.begin();
           song_it != art_it->second->albums[alb_it->first]->songs.end(); song_it++) {

        cout << "                " << song_it->second->track << ". " << song_it->second->title
             << ": " << Time_To_Str(song_it->second->time) << endl;
      }
	  */
    }
  }
  
  return 0;
}
