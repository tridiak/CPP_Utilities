//
//  MultiString.hpp
//  CPP-Utilities
//
//  Created by tridiak on 4/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef MultiString_hpp
#define MultiString_hpp

#include <stdio.h>
#include "StringStuff.hpp"

/*
A class wrapper around a string set which possesses the operators == & !=.
Also allows for case sensitivity.
*/

class MultiString {
	SST::StringSet strings;
	
	bool caseInsen;
public:
	MultiString(bool CI = false);
	MultiString(const std::string& s, bool CI = false);
	MultiString(const SST::StringArray& array, bool CI = false);
	MultiString(const SST::StringSet& set, bool CI = false);
	
	const SST::StringSet& Set() const;
	//---------------
	
	// Changing the flag will not modify the 'strings' set.
	void SetCaseInsen(bool flag);
	bool CaseInsen() const { return caseInsen; }
	
	// Case sensitivity is factored in when adding
	void AddString(const std::string& s);
	void AddStrings(const SST::StringArray& array);
	void AddStrings(const SST::StringSet& set);
	
	// Case sensitivity is factored in when removing
	void RemoveString(const std::string& s);
	void RemoveStrings(const SST::StringArray& array);
	void RemoveStrings(const SST::StringSet& set);
	
	//----------------
	// Returns true if any string in 'strings' is equal to 's'.
	// Factors in case sensitivity.
	bool operator==(const std::string& s) const;
	
	// Returns true if no string in 'strings' is equal to 's'
	// Factors in case sensitivity.
	bool operator!=(const std::string& s) const;
	
	// Factors in case sensitivity.
	bool Contains(const std::string& s) const;
};

//---------------------------


#endif /* MultiString_hpp */
