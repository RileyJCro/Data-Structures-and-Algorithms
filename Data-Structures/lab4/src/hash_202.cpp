#include "hash_202.hpp"
#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>

/* hash_202.cpp
   Riley Crockett
   September 21, 2021

   This program implements methods of the Hash_202 class, which are defined in the
     hash_202.hpp header file. 
   - The Set_Up method defines the table size, hash function type, and collision strategy.
   - The Add method attempts to insert a key-value pair into the hash table. 
   - The Find method searches for a key in the hash table and returns its value.
   - The Print method prints information on all of the non-empty hash entries.
   - The TotalProbes method used the Find method to calculate the total number of probes.

   There are 2 hash function types: 
   - Last7 (Fxn = 0) : Takes the last 7 hex characters of the key
   -  XOR  (Fxn = 1) : Splits the key into groups of 7 hex characters, and calculates the 
                       the bitwise exclusive of those numbers (last word can be smaller).
   
   There are 2 collision strategies:
   - Linear (Coll = 0)
   - Double (Coll = 1)
   */

using namespace std;

string Hash_202::Set_Up(size_t table_size, const string &fxn, const string &collision)
{
  /* Check for invalid hash table parameters and store them */

  if (!Keys.empty()) return "Hash table already set up";
  if (!table_size) return "Bad table size";
  
  if (fxn == "Last7") { Fxn = 0; } 
  else if (fxn == "XOR") { Fxn = 1; } 
  else { return "Bad hash function"; }

  if (collision == "Linear") { Coll = 0; } 
  else if (collision == "Double") { Coll = 1; } 
  else { return "Bad collision resolution strategy"; }
  
  /* Resize the key and value hash tables */

  Keys.resize(table_size);
  Vals.resize(table_size);
  return "";
}

string Hash_202::Add(const string &key, const string &val)
{
  unsigned i;                            // The iterator variable
  unsigned key_len = key.length();       // The length of the key (hex chars)
  unsigned table_size = Keys.size();     // The hash table size
  unsigned last, xor_res;                // The hash function results for Last7 and XOR
  unsigned tmp;                          // A temporory variable for substring conversion
  unsigned hash, h1, h2;                 // The hash values
  istringstream ss;                      // Stringstream for string to int conversion
  vector <string> sevens;                // String vector to store substrings
  
  /* Check for invalid hash table, key, and value parameters */

  if (Keys.empty()) return "Hash table already set up";

  if (key == "") return "Empty key";

  for (i = 0; i < key_len; i++) {
    if (!(isxdigit(key[i]))) return "Bad key (not all hex digits)";
  }

  if (val == "") return "Empty val";

  for (i = 0; i < table_size; i++) {
	if (Keys[i] == "") break;
	else if (i == table_size - 1) return "Hash table full";
  }
 
  /* Calculate the Last7 hash */

  if (Fxn == 0 || Coll == 1) {
    if (key_len >= 7) {                              // If the key length is greater than 7
	  sevens.push_back(key.substr(key_len - 7));     // Pushback the last 7 characters into the vector
	} else {                                         
	  sevens.push_back(key.substr(0));               // Otherwise pushback the whole key
	}
	ss.clear();
    ss.str(sevens[0]);                               // Pass the string to the stringstream
	ss >> hex >> last;                               // Store the hex value in last
	sevens.clear();
  }
  
  /* Calculate the XOR hash */

  if (Fxn == 1 || Coll == 1) {
	if (key_len > 7) {                                             // If the key length is greater than 7
	  for (i = 0; i < key_len / 7; i++) {  
	    sevens.push_back(key.substr(i * 7, 7));                    // Pushback groups of 7 characters to the vector               
	  } 
      if (key_len % 7) {                                           
		sevens.push_back(key.substr(key_len - (key_len % 7)));     // Pushback any remaining characters to the vector
	  }
	}
	else {
	  sevens.push_back(key.substr(0));                             // Otherwise pushback the whole key
	}                      

    for (i = 0; i < sevens.size(); i++) {                          // Loop through the vector elements
      ss.clear();
      ss.str(sevens[i]);                                           // Pass the string to the stringstream
      ss >> hex >> tmp;                                            // Store the hex value in the temp. variable
      if (i == 0) { xor_res = tmp; }                               // Set the result to tmp on the first iteration
	  else { xor_res ^= tmp; }                                     // XOR the result with every other subsection
    }
    sevens.clear();
  }
  
  /* Implement linear probing */

  if (Coll == 0) {
	for (i = 0; i < table_size; i++) {
      if (Fxn == 0) { hash = (last + i) % table_size; }                    // Set the hash based on the function type
      if (Fxn == 1) { hash = (xor_res + i) % table_size; }	  	  
	  
	  if (Keys[hash] == key) return "Key already in the table";
	  
	  if (Keys[hash] == "") {                                              // Insert the key and value at an open index
		Keys[hash] = key;                                     
        Vals[hash] = val;
		Nkeys++;
		break;
      }
    }
  }
  
  /* Implement double probing */

  if (Coll == 1) {                                      
    if (Fxn == 0) {                                                 // Set the 1st and 2nd hash based on function type  
      h1 = last % table_size;
      h2 = xor_res % table_size; 
	} 
	if (Fxn == 1) {
      h1 = xor_res % table_size;
      h2 = last % table_size; 
	}
    
	for (i = 0; i < table_size; i++) {
	  if (Keys[h1] == key) return "Key already in the table";
	  
	  if (Keys[h1] == "") {                                         // Insert the key and value if the 1st hash is open
        Keys[h1] = key;
        Vals[h1] = val;
		Nkeys++;
        return "";
      }
	  else {
		if (h2 == 0) { h2 = 1; }                                    // Set the 2nd hash increment to one if its zero  
	    hash = (h1 + h2 * i) % table_size;                          // Calculate the new hash

	    if (Keys[hash] == key) return "Key already in the table"; 
		
		if (Keys[hash] == "") {                                     // Inserts the key and value if the new hash is open
          Keys[hash] = key;
          Vals[hash] = val;
		  Nkeys++;
          return "";
		} 
      }
    }
	return "Cannot insert key";                                     // If the loop finishes without inserting a key
  }
  return "";
}

string Hash_202::Find(const std::string &key)
{
  /* The start of this method is similar to Add */

  unsigned i;                            // The iterator variable
  unsigned len = key.length();           // The length of the key (hex chars)
  unsigned table_size = Keys.size();     // The hash table size
  unsigned last, xor_res, tmp;           // The hash function results for Last7 and XOR
  unsigned hash, h1, h2;                 // A temporory variable for substring conversion 
  istringstream ss;                      // The hash values 
  vector <string> sevens;                // Stringstream for string to int conversion 
  
  /* Check for invalid key values and return an empty string */

  if (Keys.empty()) return "";
  for (i = 0; i < table_size; i++) {
    if (key == Keys[i]) break;
	else if (i + 1 == table_size) return "";
  }
  for (i = 0; i < len; i++) {
    if (!(isxdigit(key[i]))) return "";
  }
  
  /* Calculate the Last7 hash */

  if (Fxn == 0 || Coll == 1) {
    if (len >= 7) {
      sevens.push_back(key.substr(len - 7));
    } else {
      sevens.push_back(key.substr(0));
    }
    ss.clear();
    ss.str(sevens[0]);
    ss >> hex >> tmp;
    last = tmp;
    sevens.clear();
  } 
  
  /* Calculate the XOR hash */  

  if (Fxn == 1 || Coll == 1) {
    if (len > 7) {
      for (i = 0; i < len / 7; i++) {
        sevens.push_back(key.substr(i * 7, 7));
      }
      if (len % 7 != 0) {
        sevens.push_back(key.substr(len - (len % 7)));
      }
    }
    else { sevens.push_back(key.substr(0)); }

    for (i = 0; i < sevens.size(); i++) {
      ss.clear();
      ss.str(sevens[i]);
      ss >> hex >> tmp;
      if (i == 0) {
        xor_res = tmp;
      } else {
        xor_res ^= tmp;
      }
    }
    sevens.clear();
  } 
  
  /* Implement linear probing */

  if (Coll == 0) {
    for (i = 0; i < table_size; i++) {
      if (Fxn == 0) { hash = (last + i) % table_size; }
      if (Fxn == 1) { hash = (xor_res + i) % table_size; }
      if (Keys[hash] == key) {                                    // If the key is at the hash index
		Nprobes += i;                                             // Add the loop increment to Nprobes
		return Vals[hash];                                        // Return the corresponding value
	  }
    }
  }
  
  /* Implement double probing */

  if (Coll == 1) {
    if (Fxn == 0) {
      h1 = last % Keys.size();
      h2 = xor_res % Keys.size();
    }
    if (Fxn == 1) {
      h1 = xor_res % Keys.size();
      h2 = last % Keys.size();
    }

    for (i = 0; i < table_size; i++) {
      if (Keys[h1] == key) return Vals[h1];              // If the key is at the 1st hash index return its value
      
	  if (h2 == 0) { h2 = 1; }
      hash = (h1 + h2 * i) % table_size;                 // Calculate the new hash
      
	  if (Keys[hash] == key) {                           // If the key is at the new hash
		Nprobes += i;                                    // Add the loop increment to Nprobes
        return Vals[hash];                               // Return the corresponding value
      }
    }
  } 
  return "";
}

void Hash_202::Print() const
{
  unsigned i;                       // The iterator variable 

  /* Check if there are keys and loop through them.
     If a key is not empty print the table index, key, and value. */

  if (!Keys.empty()) {
    for (i = 0; i < Keys.size(); i++) {
      if (Keys[i] != "") { 
		printf("%*u %s %s\n", 5, i, Keys[i].c_str(), Vals[i].c_str());
      }
    }
  }
}

size_t Hash_202::Total_Probes()
{
  unsigned i;                      // The iterator variable 
  Nprobes = 0;                     // Reset Nprobes between calls
  
  if (Keys.empty()) return 0;      // Exit if the table is not set up

  /* Loop through the keys and call Find to total up the number of probes for each key */

  for (i = 0; i < Keys.size(); i++) {
    if (Keys[i] != "") {
	  Find(Keys[i]);
	}
  }
  return Nprobes;
}
