#include <iostream>
#include <string>
#include <cstdio>
#include <map>

using namespace std;

/**/

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
  char buf[20];
  string str;
  int m, s;

  if (total_sec == 0) return ""; // What to do--

  m = total_sec / 60;
  s = total_sec % 60;

  sprintf(buf, "%d:%d", m, s);
  str = buf;

  return str;
}

int main()
{
  string line;
  // int min, sec;
  
  int total;
  string str;

  while (getline(cin, line)) { 
    total = Str_To_Time(line);
	str = Time_To_Str(total);
	cout << str << " = " << total << " = " << str << endl;
  }

  return 0;
}
