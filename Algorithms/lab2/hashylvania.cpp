#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
using namespace std;

/* hashylvania.cpp
   Riley Crockett
   February 14, 2022

   This program reads in people with their ids, hashes and stores their
   values in maps, and prints out their reordered information.
   */

/* The hash function */

unsigned long long jsp_hash(const string &s)
{
  size_t i;
  unsigned long long h, t;

  h = 0x6bd300f29f1505ULL;

  for (i = 0; i < s.size(); i++) {
    t = h & 0x3ffffffffffffffULL;
    h = (t << 5) ^ h ^ s[i];
  }
  return h;
}

/* A Person struct to hold a person's name and id */

struct Person { 
  string name, id;
};

int main()
{
  stringstream ss;                    // A stringstream to parse std input
  string line, str;                   // Strings to hold input lines and parsed strings
  string hash_str, sid;               // The string to hash, and the name-id-string combination
  vector <Person *> people;           // A vector of pointers to Person structs
  unsigned long long h, mod, hash;    // The calculated hash, given modulus, and a person's hash
  
  /* There are two maps: 
     pm - A map with Person pointers keyed by a person's hash
	  m - A map with pm maps (person maps) keyed by a person's hash modulo the given modulus */

  map <unsigned long long, Person *> pm;
  map <unsigned long long, Person *>::iterator pm_it;
  map <unsigned long long, map <unsigned long long, Person *> > m;
  map <unsigned long long, map <unsigned long long, Person *> >::iterator mit;

  /* Read from standard input */

  while (getline(cin, line)) {
	ss.clear();                   // Clear the stringstream
	ss.str(line);                 // Pass input to the line string

	/* Parse the individual strings */

	while (ss >> str) {
	  if (str == "PERSON") {
        Person *p = new Person;       // Create a new person instance
        ss.clear(); ss >> p->name;    // Store their name
		ss.clear(); ss >> p->id;      // Store their id
        people.push_back(p);          // Push the person to the people vector
      } 
	  else if (str == "ORDER") {
        ss.clear(); ss >> hash_str;    // Store the random string
        ss.clear(); ss >> mod;         // Store the given modulus

		/* Traverse every person in the people vector */
        
        for (size_t i = 0; i < people.size(); i++) {
		  sid = people[i]->name + people[i]->id + hash_str;    // Store the name-id-string combination
          hash = jsp_hash(sid);                                // Calculate a person's hash
		  
		  pm.clear();                               // Clear the people map                          
          pair<unsigned long long, Person *> ps;    // Make a pair with a person and their hash
		  ps = make_pair(hash, people[i]);     
		  pm.insert(ps);                            // Insert the pair in the people map 
		  
		  /* See if the current person's hash modulo the modulus is already a key in the outer map. 
		     If it is, insert the pair under the existing modulo hash key. Otherwise, create a new 
			 pair for the outer map, with the new hash modulus and current people map. */

		  mit = m.find(hash % mod);
		  if (mit != m.end()) { 
			mit->second.insert(ps);  
		  } else { 
			m.insert(pair<unsigned long long, map <unsigned long long, Person *> >(hash % mod, pm));
		  }
		}

		unsigned long long mod_hash;    // Stores the hash modulo the modulus
		unsigned long long right;       // Used for comparing how close a key is to the hash
		Person* first = NULL;           // A Person pointer for the first person
        Person* last = NULL;            // A Person pointer for the last person

		h = jsp_hash(hash_str);      // Hash of the random string

		/* Loop until the outer map is empty */

		while (!m.empty()) {

		mod_hash = h % mod;
		  
		 /* Set the outer map iterator to the smallest value greater than the modulo hash */

          mit = m.lower_bound(mod_hash);

		  /* If no such value was found (end), decrement the iterator to the greatest value.
		     Otherwise, if the iterator is not at the start, save the lower bound value and 
			 decrement the iterator. If the stored value is closer to the modulo hash than 
			 the current/lesser value, reincrement the iterator. */

		  if (mit == m.end()) {
            mit--;
          } else if (mit != m.begin()) {
			right = mit->first;
		    mit--;
            if (right - mod_hash <= mod_hash - mit->first) mit++;
		  }

		  /* Set the people map iterator to the lower bound result of the hash */
		  
		  pm_it = mit->second.lower_bound(h);

		  /* If there is no hash >= the lower bound result set the iterator to the greatest hash */
		  
		  if (pm_it == mit->second.end()) pm_it--;
          
		  /* Store the first person if it's value has not been set. 
		     Store the last person when the outer and inner map sizes are 1. */

		  if (first == NULL) first = pm_it->second;
		  if (mit->second.size() == 1 && m.size() == 1) last = pm_it->second;
    	
		  /* Convert the current hash to a hex string, and concatenate it to the person's id. 
		     Calculate the next hash by calling the hash function on the concatenated string. */

		  stringstream sstream;
		  sstream.clear(); sstream << hex << h;
          hash_str = pm_it->second->id + sstream.str();
		  h = jsp_hash(hash_str);
		  
		  mit->second.erase(pm_it);                      // Erase the person's entry from the inner map
          if (mit->second.size() == 0) m.erase(mit);     // When the inner map is empty, erase it from the outer map
        }

		/* After ordering, print the first and last person with their name and id */
		
		printf("Last: %s %s\n", last->name.c_str(), last->id.c_str());
        printf("First: %s %s\n", first->name.c_str(), first->id.c_str());
	  }
	}
  }

  /* Free the memory allocated for the Person instances */

  for (size_t i = 0; i < people.size(); i++) delete people[i];

  return 0;
}
