//
//  MultiString.cpp
//  CPP-Utilities
//
//  Created by tridiak on 4/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "MultiString.hpp"
#include "Converters.hpp"
#include <set>

MultiString::MultiString(bool CI) {
	caseInsen = CI;
};

MultiString::MultiString(const std::string& s, bool CI) {
	caseInsen = CI;
	strings.insert(s);
};

MultiString::MultiString(const SST::StringArray& array, bool CI) {
	caseInsen = CI;
	strings = ArrayToSet(array);
};

MultiString::MultiString(const SST::StringSet& set, bool CI) {
	caseInsen = CI;
	strings = set;
};

const SST::StringSet& MultiString::Set() const {
	return strings;
};

//---------------
void MultiString::SetCaseInsen(bool flag) {
	caseInsen = flag;
};

// CI is factored in when adding
void MultiString::AddString(const std::string& s) {
	if (*this != s) { strings.insert(s);}
};

void MultiString::AddStrings(const SST::StringArray& array) {
	for (auto itr = array.begin(); itr != array.end(); itr++) {
		AddString(*itr);
	}
};

void MultiString::AddStrings(const SST::StringSet& set) {
	for (auto itr = set.begin(); itr != set.end(); itr++) {
		AddString(*itr);
	}
};

void MultiString::RemoveString(const std::string& s) {
	if (!caseInsen) { strings.erase(s); }
	else {
		for (auto itr = strings.begin(); itr != strings.end(); itr++) {
			if (SST::RPStringEqualCaseInsen(*itr, s)) {
				strings.erase(itr);
				break;
			}
		}
	}
};

void MultiString::RemoveStrings(const SST::StringArray& array) {
	for (auto itr = array.begin(); itr != array.end(); itr++) {
		RemoveString(*itr);
	}
};

void MultiString::RemoveStrings(const SST::StringSet& set) {
	for (auto itr = set.begin(); itr != set.end(); itr++) {
		RemoveString(*itr);
	}
};

//----------------
// Returns true if any string in 'strings' is equal to 's'.
bool MultiString::operator==(const std::string& s) const {
	return Contains(s);
};

bool MultiString::operator!=(const std::string& s) const {
	return !Contains(s);
};

bool MultiString::Contains(const std::string& s) const {
	if (!caseInsen) { return strings.find(s) != strings.end(); }
	for (auto itr = strings.begin(); itr != strings.end(); itr++) {
		if (SST::RPStringEqualCaseInsen(*itr, s)) { return true; }
	}
	
	return false;
};
