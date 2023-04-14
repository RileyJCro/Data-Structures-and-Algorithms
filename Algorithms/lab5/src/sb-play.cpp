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

/* sb-play.cpp
   Riley Crockett
   March 21, 2022

   This program outputs a valid move for a Superball game instance.
   The update_swap_rating() function is where the valid swaps are rated.
   It takes 2 swap indices, a disjoint set reference, and a vector for 
   rating information ([rating, index1, index2] format).

   (NOTE) It takes a signficant amount of time to run but I think it's 
   under the 5s per 1000 score mention.
   
   sh scripts/run_multiple.sh 8 10 5 pbyrg bin/sb-play 100 1
   2153.36s user 43.23s system 91% cpu 40:12.57 total
   Avg. Score: 9151.900
   */

class Superball {
  public:
    Superball(int argc, char **argv);
    void analyze_superball(int index, int id, Disjoint_Set &d);
	void update_swap_rating(int s1, int s2, Disjoint_Set &d, vector <int> &rating);
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
  if (board[index] == board[id] && d.Find(index) != index) return;
  if (board[index] + 32 != board[id] && board[index] != board[id]) return;
  if (index != id) d.Union(d.Find(index), d.Find(id));
  
  if (index % c != 0) analyze_superball(index-1, id, d);
  if ((index+1) % c != 0) analyze_superball(index+1, id, d);
  if (index - c >= 0) analyze_superball(index-c, id, d);
  if (index + c <= (int)board.size()) analyze_superball(index+c, id, d);
}

void Superball::update_swap_rating(int s1, int s2, Disjoint_Set &d, vector <int> &rating)
{
  Disjoint_Set ds;                
  ds.Initialize(board.size());    // Another disjoint set instance for rating the swap

  int i, tmp;          // An iterator and a temp int for swapping the board indices
  int n_rating = 0;    // An int for the new rating calculation
  int ds_sz, d_sz;     // Ints for the disjoint set sizes
  
  tmp = board[s1]; 
  board[s1] = board[s2];    // Swap the two board indices
  board[s2] = tmp;     
  
  /* Call analyze on the non-empty goal cells */

  for (i = 0; i < r*c; i++) {
    if (goals[i] && board[i] != '*') analyze_superball(i, i, ds);
  }  
  
  /* Loop through the board. On a goal cell, if the sizes don't match adjust the 
     new rating. */

  for (i = 0; i < (int)ds.Size(); i++) {
    ds_sz = (*ds.Get_Sizes())[i];
    d_sz = (*d.Get_Sizes())[i];
	if (goals[i] && board[i] != '*') {
	  if (ds_sz != d_sz) {
	    n_rating += 6 * (ds_sz - d_sz);
	  } else {
	    n_rating += 1;
	  }
	}
  }

  /* Call analyze for the board */

  for (i = 0; i < r*c; i++) analyze_superball(i, i, ds);
  
  /* Add the difference in total set size to the new rating */

  n_rating += 6 * ((*d.Get_Set_Ids()).size() - (*ds.Get_Set_Ids()).size());

  /* Update the rating information if a greater rating value is found */

  if (n_rating >= rating[0]) {
	rating.clear();
	rating = {n_rating, s1, s2};
  }

  tmp = board[s1];
  board[s1] = board[s2];    // Swap the two board indices back
  board[s2] = tmp;   
}

int main(int argc, char **argv)
{
  Disjoint_Set d;
  Superball *s;

  s = new Superball(argc, argv);    // A superball instance
  d.Initialize(s->board.size());    // A disjoint set instance for the board

  int i, j, sz;    // Two iterators and an int for set size
  
  /* Call analyze for the whole board */

  for (i = 0; i < s->r*s->c; i++) s->analyze_superball(i, i, d);

  vector <int> set_ids = (*d.Get_Set_Ids());    // A vector for the disjoint set ids
  
  vector <int> valid_swaps;      // A vector for the valid swaps
  vector <int> valid_scores;     // A vector for the valid scores
  list <int> els;                // A list for set elements
  list <int>::iterator lit;      // The element iterator

  /* Loop through the sets and their elements. If the set has a goal and its size
     is greater than the min score size, store the valid score. Otherwise if an
	 element is not empty, store it as a valid swap. */

  for (i = 0; i < (int)set_ids.size(); i++) {
    sz = (*d.Get_Sizes())[set_ids[i]];
	els = (*d.Get_Elements())[set_ids[i]];
	for (lit = els.begin(); lit != els.end(); lit++) {
      if (sz >= s->mss && s->goals[*lit] && s->board[*lit] != '*') {
        valid_scores.push_back(*lit);
      } else if (s->board[*lit] != '*' && s->board[*lit] != '.') {
        valid_swaps.push_back(*lit);
      }
    }
  }

  /* A vector to hold swap rating information */

  vector <int> swap_rating = {-5, valid_swaps[0], valid_swaps[1]};
 
  /* Loop through the valid swaps to permute them and call the update rating function */

  if (s->empty >= s->mss) {
    for (i = 0; i < (int)valid_swaps.size(); i++) {
      for (j = i+1; j < (int)valid_swaps.size(); j++) {
        s->update_swap_rating(valid_swaps[i], valid_swaps[j], d, swap_rating);
	  }
    }
  }

  /* If you can score but there are 5 or more empty cells, you swap, otherwise you score. 
     If you can't score you swap to end the game. */

  if (!valid_scores.empty() && s->empty < s->mss) {
    printf("SCORE %d %d\n", valid_scores[0]/s->c, valid_scores[0]%s->c);
  } else {
    printf("SWAP %d %d %d %d\n", swap_rating[1]/s->c, swap_rating[1]%s->c, swap_rating[2]/s->c, swap_rating[2]%s->c);   
  }
  
  return 0;
}
