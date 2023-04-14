#include "disjoint_set.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <vector>
using namespace std;
using plank::Disjoint_Set;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

/* sb-analyze.cpp
   Riley Crockett
   March 21, 2022

   This program analyzes and prints scoring set information of a Superball game instance.
   The analyze_superball() function and main are located at the bottom of the program.

   - analyze_superball() takes an index, an id, and a disjoint set reference.
     It's a recursive function that checks if the surrounding cells are the same color
     as the id/original cell.
   */

class Superball {
  public:
    Superball(int argc, char **argv);
    void analyze_superball(int index, int id, Disjoint_Set &d);
	int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
	vector <int> goals;
    vector <int> colors;
};

void usage(const char *s)
{
  fprintf(stderr, "usage: sb-read rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < (int)strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
	if ((int)s.size() != c) {
	  fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

void Superball::analyze_superball(int index, int id, Disjoint_Set &d)
{  
  // Return if the index and id chars match and the index is already in a set
  if (board[index] == board[id] && d.Find(index) != index) return;

  // Return if the index char is not the id char or its lowercase
  if (board[index] + 32 != board[id] && board[index] != board[id]) return;
  
  // Make a union between the index and id if they are not the same value
  if (index != id) d.Union(d.Find(index), d.Find(id));

  /* If the cell is not at the edge of the board, recursively call 
     in the left, right, up, and down direction respectively */

  if (index % c != 0) analyze_superball(index-1, id, d);
  if ((index+1) % c != 0) analyze_superball(index+1, id, d);  
  if (index - c >= 0) analyze_superball(index-c, id, d);
  if (index + c <= (int)board.size()) analyze_superball(index+c, id, d);
}

int main(int argc, char **argv)
{
  Disjoint_Set d;
  Superball *s;
  
  s = new Superball(argc, argv);    // A superball instance
  d.Initialize(s->board.size());    // A disjoint set with the board size

  int i, sz;                   // An iterator and an int for set size
  list <int> els;              // A list for set elements
  list <int>::iterator lit;    // The element list iterator

  /* Call analyze for the whole board */

  for (i = 0; i < s->r*s->c; i++) s->analyze_superball(i, i, d);

  vector <int> set_ids = (*d.Get_Set_Ids());     // A vector for the disjoint set ids

  /* Loop through each sets elements. If the set size is greater than the min 
     score size and it contains a goal, print out the size, char, and goal index. */

  printf("Scoring sets:\n");

  for (i = 0; i < (int)set_ids.size(); i++) {
    sz = (*d.Get_Sizes())[set_ids[i]];
    els = (*d.Get_Elements())[set_ids[i]];

    for (lit = els.begin(); lit != els.end(); lit++) {
      if (sz >= s->mss && s->goals[*lit] && s->board[*lit] != '*') {
        printf("Size: %2d  Char: %c  Scoring Cell: %d,%d\n", sz, s->board[*lit], *lit/s->c, *lit%s->c);
        break;
      }
    }
  }

  return 0;
}
