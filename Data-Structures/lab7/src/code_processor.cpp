#include "code_processor.hpp"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

/*  code_processor.cpp
    Riley Crockett
	October 26, 2021

	This program supports a reward system by implementing the class methods defined in
    the code_processor.hpp header file. The header file also contains User and Prize classes to 
	store prize and user instances. There are 3 maps: Names, Prizes, and Phones that store
	pointers to User/Prize instances. There are two sets: phone_numbers (specific to a user),
	and Codes that store phone numbers and codes respectively. 
	*/

/* Implement the djb hash function */

unsigned int djbhash(const string &s)
{
  size_t i;
  unsigned int h;

  h = 5381;

  for (i = 0; i < s.size(); i++) {
    h = (h << 5) + h + s[i];
  }
  return h;
}

/* Create a new prize from given parameters */

bool Code_Processor::New_Prize(const string &id, const string &description, int points, int quantity)
{
  map <string, Prize *>::iterator prit;     // The Prizes map iterator
  Prize *prz;                               // A Prize pointer

  if (points <= 0) return false;            
  if (quantity <= 0) return false;
  prit = Prizes.find(id);
  if (prit != Prizes.end()) return false;   // Return false if the Prize id is already in the map
  
  prz = new Prize;                          // Create a new prize instance
  prz->id = id;
  prz->description = description;
  prz->points = points;
  prz->quantity = quantity;

  Prizes.insert(make_pair(id, prz));        // Insert the Prize id-pointer pair into the map
  return true;
}

/* Create a new user from given parameters */

bool Code_Processor::New_User(const string &username, const string &realname, int starting_points)
{
  map <string, User *>::iterator uit;          // The Names map iterator
  User *usr;                                   // A user pointer

  if (starting_points < 0) return false;
  uit = Names.find(username);
  if (uit != Names.end()) return false;        // Return false if the username is taken
 
  usr = new User;                              // Create a new user instance
  usr->username = username;
  usr->realname = realname;
  usr->points = starting_points;

  Names.insert(make_pair(username, usr));      // Insert the username-pointer pair into Names
  return true;
}

/* Delete a user and erase their phone numbers */

bool Code_Processor::Delete_User(const string &username)
{
  map <string, User *>::iterator uit;      // The Names map iterator
  map <string, User *>::iterator phonit;   // The Phones map iterator
  set <string>::iterator uphit;            // The phone_numbers set iterator
  
  uit = Names.find(username);
  if (uit == Names.end()) return false;    // Return false if the user does not exist

  /* Loop through the user's phone numbers and erase them */

  for (uphit = uit->second->phone_numbers.begin(); 
	   uphit != uit->second->phone_numbers.end(); uphit++) {
	Phones.erase(*uphit);
  }

  Names.erase(username);                   // Erase the username from Names
  delete uit->second;                      // Delete the user pointer
  return true;
}

/* Register a user's phone number */

bool Code_Processor::Add_Phone(const string &username, const string &phone)
{
  map <string, User *>::iterator uit;               // The Names map iterator
  set <string>::iterator uphit;                     // The phone_numbers set iterator
  map <string, User *>::iterator phonit;            // The Phones map iterator

  /* Return false if the user does not exist, or the phone number already exists */

  uit = Names.find(username);
  if (uit == Names.end()) return false;
  
  uphit = uit->second->phone_numbers.find(phone);
  if (uphit != uit->second->phone_numbers.end()) return false;

  phonit = Phones.find(phone);
  if (phonit != Phones.end()) return false;

  uit->second->phone_numbers.insert(phone);         // Insert phone into user's phone #'s
  Phones.insert(make_pair(phone, uit->second));     // Insert phone-name pair into Phones
  return true;
}

/* Remove a phone from the system */

bool Code_Processor::Remove_Phone(const string &username, const string &phone)
{
  map <string, User *>::iterator uit;         // The Names map iterator
  set <string>::iterator uphit;               // The phone_numbers set iterator
  map <string, User *>::iterator phonit;      // The Phones map iterator

  /* Return false if the user or phone number does not exist */

  uit = Names.find(username);
  if (uit == Names.end()) return false;
  
  uphit = uit->second->phone_numbers.find(phone);
  if (uphit == uit->second->phone_numbers.end()) return false;
  
  phonit = Phones.find(phone);
  if (phonit == Phones.end()) return false;

  uit->second->phone_numbers.erase(phone);    // Erase the phone from user's phone #'s 
  Phones.erase(phone);                        // Erase the phone-name pair from Phones
  return true;
}

/* Return a string containing all of a user's phone numbers */

string Code_Processor::Show_Phones(const string &username) const
{
  map <string, User *>::const_iterator uit;     // The Names map iterator
  set <string>::const_iterator uphit;           // The phone_numbers set iterator 
  string phone_string;                          // A string to store the phone numbers

  uit = Names.find(username);
  if (uit == Names.end()) return "BAD USER";    // Return error string if the user DNE
  
  /* Loop through the user's phone numbers and append them to the phone# string */

  for (uphit = uit->second->phone_numbers.begin(); 
	   uphit != uit->second->phone_numbers.end(); uphit++) {
    phone_string += *uphit;
	phone_string += "\n";
  }
  return phone_string;
}

/* Store a new code based on a username and adjust the user's points */

int Code_Processor::Enter_Code(const string &username, const string &code)
{
  set <string>::iterator cit;            // The Codes set iterator
  map <string, User *>::iterator uit;    // The Names map iterator
  unsigned int code_hash;                // An unsigned int to store a code's djb hash 
  int code_val;                          // An integer to store a code's point value
  
  /* Return -1 if the code already exists, or the user does not exist */

  cit = Codes.find(code);
  if (cit != Codes.end()) return -1;
  
  uit = Names.find(username);
  if (uit == Names.end()) return -1;

  code_hash = djbhash(code);             // Store the code's djb hash
  
  /* Assign the point value associated with a hash's divisibility */

  if (code_hash % 17 == 0) {
    code_val = 10;
  } else if (code_hash % 13 == 0) {
    code_val = 3;
  } else {
    return 0;
  }

  Codes.insert(code);                    // Insert the code into the Codes set
  uit->second->points += code_val;       // Increment user's points by the code value 
  return code_val;
}

/* Store a new code based on a phone number and adjust the user's points */

int Code_Processor::Text_Code(const string &phone, const string &code)
{
  set <string>::iterator cit;               // The Codes set iterator
  map <string, User *>::iterator phonit;    // The Phones map iterator
  set <string>::iterator uphit;             // The phone_numbers set iterator
  unsigned int code_hash;                   // An unsigned int to store a code's djb hash
  int code_val;                             // An integer to store a code's point value

  /* Return -1 if the code already exists, or the phone does not exist */

  cit = Codes.find(code);
  if (cit != Codes.end()) return -1;

  phonit = Phones.find(phone);
  if (phonit == Phones.end()) return -1;

  code_hash = djbhash(code);                // Store the code's djb hash

  /* Assign the point value associated with a hash's divisibility */

  if (code_hash % 17 == 0) {
    code_val = 10;
  } else if (code_hash % 13 == 0) {
    code_val = 3;
  } else {
    return 0;
  }

  Codes.insert(code);                       // Insert the code into the Codes set
  phonit->second->points += code_val;       // Increment user's points by the code value
  return code_val;
}

/* Mark a code as used */

bool Code_Processor::Mark_Code_Used(const string &code)
{
  set <string>::iterator cit;                  // The Codes set iterator   
  unsigned int code_hash;                      // An unsigned int to store a code's djb hash

  cit = Codes.find(code);
  
  /* If the code does not exist do the following, otherwise return false */

  if (cit == Codes.end()) {
    code_hash = djbhash(code);                 // Store the code's djb hash             
    
	/* Store the code if it is valid, otherwise return false */

	if (code_hash % 17 == 0 || code_hash % 13 == 0) {
      Codes.insert(code);                        
    } else {
	  return false;
	}
  }
  else {
    return false;
  }
  return true;
}

/* Return a user's balance */

int Code_Processor::Balance(const string &username) const
{
  map <string, User *>::const_iterator uit;    // The Names map iterator

  uit = Names.find(username);
  if (uit == Names.end()) return -1;           // If the user DNE return false

  return uit->second->points;                  // Return the user's points
}

/* Update User and Prize values when a user redeems a prize */

bool Code_Processor::Redeem_Prize(const string &username, const string &prize)
{
  map <string, User *>::iterator uit;             // The Names map iterator
  map <string, Prize *>::iterator prit;           // The Prizes map iterator

  /* Return false if the user or prize DNE */

  uit = Names.find(username);
  if (uit == Names.end()) return false;

  prit = Prizes.find(prize);
  if (prit == Prizes.end()) return false;

  /* Return false if the user does not have enough points for the prize */

  if (uit->second->points < prit->second->points) return false;
  
  uit->second->points -= prit->second->points;    // Decrement User's points by Prize's points
  prit->second->quantity--;                       // Decrement the Prize quantity
 
  /* If the Prize quantity is 0, erase it from Prizes, and delete the Prize pointer */

  if (prit->second->quantity == 0) {
	Prizes.erase(prit);
	delete prit->second; 
  }
  return true;
}

/* Destructor for the Code_Processor class */

Code_Processor::~Code_Processor()
{
  map <string, User *>::iterator uit;      // The Names map iterator
  map <string, Prize *>::iterator prit;    // The Prizes map iterator

  /* Loop through every element of the Names map and delete the User pointer */

  for (uit = Names.begin(); uit != Names.end(); uit++) delete uit->second;

  /* Loop through every element of the Prizes map and delete the Prize */

  for (prit = Prizes.begin(); prit != Prizes.end(); prit++) delete prit->second;
}

/* */

bool Code_Processor::Write(const string &filename) const
{
  ofstream fout;                                 // The output file stream
  size_t i;
  map <string, User *>::const_iterator uit;      // The Names map iterator
  set <string>::const_iterator uphit;            // The phone_numbers iterator
  map <string, Prize *>::const_iterator prit;    // The Prizes map iterator
  set <string>::const_iterator cit;              // The Codes map iterator

  stringstream ss;                               // A stringstream object
  string pts;                                    // A string to store User and Prize points
  string qnty;                                   // A string to store Prize quantity
  string instr;                                  // A string to store instructions
  string phone_instr;                            // A string to store phone-related instructions
  vector <string> cmd_vec;                       // A vector to store the instructions

  fout.open(filename.c_str());
  if (fout.fail()) return false;                 // Return false if the file can't open

  /* Loop through the Names map */

  for (uit = Names.begin(); uit != Names.end(); uit++) {

	/* Set the instruction string to the cmd and username */
	
	instr = "ADD_USER " + uit->first + " ";

	ss.clear();                                  // Clear the sstream error state
	ss << uit->second->points;                   // Pass the User's points to the sstream
	pts = ss.str();                              // Store the User's points
	ss.str("");                                  // Clear the sstream's contents
	
	/* Append the points and realname to the instruction string */

	instr += pts + " " + uit->second->realname + "\n";

	cmd_vec.push_back(instr);                    // Store the instruction in the vector
 
	/* Loop through a user's phone numbers */

    for (uphit = uit->second->phone_numbers.begin(); 
		 uphit != uit->second->phone_numbers.end(); uphit++) {
	  
	  /* Set the phone instruction string to the cmd, username, and phone number */

	  phone_instr = "ADD_PHONE " + uit->first + " " + *uphit + "\n";
	  
	  cmd_vec.push_back(phone_instr);            // Store the instruction in the vector
	}
  }
  
  /* Loop through the Prizes map */

  for (prit = Prizes.begin(); prit != Prizes.end(); prit++) {

	/* Set the instruction string to the cmd and prize id */
    
	instr = "PRIZE " + prit->first + " ";
    
	ss.clear();                                  // Clear the sstream error state
    ss << prit->second->points;                  // Pass the Prize's points to the sstream
    pts = ss.str();                              // Store the Prize's points
	ss.str("");                                  // Clear the sstream's contents
	ss.clear();                                  // Clear the sstream error state
    ss << prit->second->quantity;                // Pass the Prize's quantity to the sstream
    qnty = ss.str();                             // Pass the quantity to the sstream
	ss.str("");                                  // Clear the sstream's contents
    
	/* Append the Prize's points, quantity, and description to the instruction */

	instr += pts + " " + qnty + " " + prit->second->description + "\n";
    
	cmd_vec.push_back(instr);                    // Store the instruction in the vector
  }
  
  /* Loop through the Codes map */

  for (cit = Codes.begin(); cit != Codes.end(); cit++) 
  {
	/* Set the instruction string to the cmd, and code */
    
	instr = "MARK_USED " + *cit + "\n";
	
	cmd_vec.push_back(instr);                    // Store the instruction in the vector
  }

  /* Loop through the cmd vector and write the instructions to the file */

  for (i = 0; i < cmd_vec.size(); i++) fout << cmd_vec[i] << endl;

  return true;
}
