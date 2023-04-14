#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <map>

using namespace std;

/* lib_info.cpp
   Riley Crockett
   February 7, 2022

   This program is used to help parse a "music library".
   -It reads from a file that has MP3 information in a specific format,
     and stores this information in Artist, Album, and Song objects.
   -After reading the MP3 file, it prints the information in a condensed format.
   */

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
  int m, s;    // Integers for minutes and seconds

  /* Split the time format to get minutes and seconds */
  
  sscanf(str.c_str(), "%d:%d", &m, &s);
  
  m *= 60;         // Multiply the minutes by 60s
  return m + s;    // Return the total # of seconds
}

// string Time_To_Str(int total_sec) 
string Time_To_Str(int total_sec)
{
  char buf[10];    // Allocate a char buffer for sprintf
  string str;      // A string to be returned
  int m, s;        // Integers for minutes and seconds

  /* Convert the total seconds to minutes/seconds format */

  m = total_sec / 60;    
  s = total_sec % 60;
  
  /* Convert minutes and seconds to string format */
  
  sprintf(buf, "%d:%02d", m, s);    // Write to the buffer
  str = buf;                        // Set the string to the buffer

  return str;
}

/* A procedure that changes underscores to spaces for a string */

void UnderscoreToSpace(string &str)
{
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] == '_') str[i] = ' ';
  }
}

int main(int argc, char *argv[])
{
  ifstream fin;             // An input filestream
  istringstream ss;         // A stringstream for parsing words
  string fileName, line;    // Strings for the file name and storing lines
  string time, genre;       // String for parsing the song time and genre

  map <string,Artist> artists;             // A map to hold every artist  
  map <string,Artist>::iterator art_it;    // The artists map iterator
  map <string,Album>::iterator alb_it;     // The albums map iterator
  map <int, Song>::iterator song_it;       // The songs map iterator

  if (argc > 2) return -1;       // Exit if more than 2 arguments are passed in
  fileName = argv[1];            // Store the file name
  fin.open(fileName.c_str());    // Attempt to open the file
  if (fin.fail()) return -1;     // Exit if the file failed to open

  /* Loop through the file line by line */

  while (getline(fin, line)) { 
    
	ss.clear();       // Clear the stringstream
	ss.str(line);     // Store the current line

    Artist artist;    // Create artist, album, and song instances
    Album album;
	Song song;

	album.time = 0;    // Initialize the album's time to '0'
	
    /* The line format is: 
	   -> Song title, song time, artist name, album name, genre, and track.
       
	   Read in each component, replace the underscores in the names 
	   with spaces, and convert the time string to an integer. */

	ss >> song.title;   UnderscoreToSpace(song.title);
	ss >> time;         song.time = Str_To_Time(time);
	ss >> artist.name;  UnderscoreToSpace(artist.name);
	ss >> album.name;   UnderscoreToSpace(album.name);
	ss >> genre;
	ss >> song.track;

    /* If there is a new artist, insert them and update the artists iterator */

	art_it = artists.find(artist.name);

	if (art_it == artists.end()) {
	  artists.insert(pair<string,Artist>(artist.name,artist));
	  art_it = artists.find(artist.name);
	} 

	/* If there is a new album, insert it and update the albums iterator */
	
	alb_it = art_it->second.albums.find(album.name);

	if (alb_it == art_it->second.albums.end()) {
	  art_it->second.albums.insert(pair<string,Album>(album.name,album));
	  alb_it = art_it->second.albums.find(album.name);
	}

	/* Insert the song under the right artist and album */
    
    alb_it->second.songs.insert(pair<int,Song>(song.track,song));

	art_it->second.time += song.time;  // Update the artist's time
	alb_it->second.time += song.time;  // Update the album's time
	art_it->second.nsongs++;           // Update the artist's number songs
  }

  fin.close();    // Close the file after reading every line
   
  /* Loop through every artist, print their name, number of songs, and total song time */

  for (art_it = artists.begin(); art_it != artists.end(); art_it++) {
    
    cout << art_it->first << ": " << art_it->second.nsongs << ", " 
		 << Time_To_Str(art_it->second.time) << endl;
    	
	/* Loop through every artist's albums, print the name, number of songs, and total song time */
    
	for (alb_it = art_it->second.albums.begin(); alb_it != art_it->second.albums.end(); alb_it++) {
	  cout << "        " << alb_it->first << ": " << alb_it->second.songs.size()
	       << ", " << Time_To_Str(alb_it->second.time) << endl;

	  /* Loop through every album's songs, print the track number, title, and time */
      
	  for (song_it = alb_it->second.songs.begin(); song_it != alb_it->second.songs.end(); song_it++){
        cout << "                " << song_it->first << ". " << song_it->second.title
		     << ": " << Time_To_Str(song_it->second.time) << endl;
      }
    }
  }

  return 0;
}
