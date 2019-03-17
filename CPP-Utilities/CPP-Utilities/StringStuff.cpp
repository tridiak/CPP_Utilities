//
//  StringStuff.cpp
//  UtilityLib
//
//  Created by Mark Noble on 8/01/17.
//  Copyright © 2017 Mark Noble. All rights reserved.
//

#include "StringStuff.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>

namespace StringStuff {

bool RPStringInArray(const StringArray& ary, const std::string& s, bool caseInsen) {
	if (s.empty() || ary.empty()) {return false;}
	std::string r = s;
	if (caseInsen) {RPStringLowercase(r);}
	return std::find(ary.begin(), ary.end(), r) != ary.end();
};

bool RPStringArrayInArray(const StringArray& ary, const StringArray& toFind, bool caseInsen) {
	if (toFind.empty() || ary.empty()) {return false;}
	
	for (auto itr = toFind.begin(); itr != ary.end(); itr++) {
		if (RPStringInArray(ary, *itr, caseInsen)) {return true;}
	}
	return false;
};

std::string RPStringBetween(const std::string& s, const std::string& open, const std::string& close, long& openPos, long& closePos) {
	size_t slen = s.length();
	openPos = -1;
	closePos = -1;
	// Zero length string
	if (slen == 0) {return "";}
	
	// If open and close markers are >= string length then it cannot return anything.
	if (open.length() + close.length() >= slen) {return "";}
	
	// Limit max string length
	if (slen >= LONG_MAX) {return "";}
	
	size_t idx1 = NPOS, idx2 = NPOS;
	if (open.empty() && close.empty()) {return s;}
	
	// If open empty, open position is start of string
	if (open.empty()) {
		idx1 = 0;
	}
	else {
		idx1 = s.find(open);
		if (idx1 == NPOS) {return "";}
	}
	
	openPos = idx1;
	
	// If close is empty than close position is end of string
	if (close.empty()) {
		return s.substr(idx1+open.length());
	}
	
	idx2 = s.find(close, idx1+open.length());
	if (idx2 == NPOS) {
		return "";
	}
	
	closePos = idx2;
	return s.substr(idx1 + open.length(), idx2 - (idx1 + open.length()) );
};

bool RPStringEqualCaseInsen(const std::string& s, const std::string& r) {
	std::string R = r;
	std::string S = s;
	RPStringLowercase(R);
	RPStringLowercase(S);
	return R == S;
};

void RPStringLowercase(std::string& s) {
	for (size_t t=0; t<s.length(); t++) {
		s[t] = tolower(s[t]);
	}
};

void RPStringUpper(std::string& s) {
	for (size_t t=0; t<s.length(); t++) {
		s[t] = toupper(s[t]);
	}
};

std::string RPRemoveIfLastChar(const std::string& s, char c) {
	if (s.empty()) { return ""; }
	if (s[s.size() - 1] == c) { return s.substr(0, s.size() - 1); }
	return s;
};

char RPLastChar(const std::string& s) {
	if (s.empty()) { return 0; }
	return s[s.size() - 1];
};

void RPStringRemoveFromSet(std::string& s, const std::string& cset, bool invert) {
	std::string::iterator itr;
	if (invert) {
		for (itr = s.begin(); itr != s.end();) {
			if (cset.find(*itr) == NPOS) {
				itr = s.erase(itr);
			}
			else { itr++;}
		}
	}
	else {
		for (itr = s.begin(); itr != s.end();) {
			if (cset.find(*itr) != NPOS) {
				itr = s.erase(itr);
			}
			else { itr++;}
		}
	}
};

StringArray RPStringSplit(const std::string& s, const std::string& divider) {
	if (divider.empty()) {return StringArray({s});}
	if (s.empty()) {return StringArray();}
	std::string r = s;
	StringArray ary;
	size_t pos;
	
	while (true) {
		pos = r.find(divider);
		if (pos == NPOS) {
			ary.push_back(r);
			return ary;
		}
		ary.push_back(r.substr(0, pos));
		r = r.substr(pos + divider.length());
	}
	
};

std::string RPStringJoin(const StringArray& sv, const std::string& join) {
	if (sv.empty()) {return "";}
	if (sv.size()==1) {return sv[0];}
	std::string r = "";
	size_t t;
	for (t=0; t<sv.size()-1; t++) {
		r = r + sv[t];
		r = r + join;
	}
	r = r + sv[t];
	return r;
};

void RPStringRemoveLeading(std::string& s, const std::string& cset) {
	if (s.empty() || cset.empty()) {return;}
	while (!s.empty() && (cset.find(s[0]) != NPOS)) {
		s.erase(s.begin());
	}
};

void RPStringRemoveLeading(std::string& s, const std::set<char>& cset) {
	if (s.empty() || cset.empty()) {return;}
	while (!s.empty() && (cset.find(s[0]) != cset.end())) {
		s.erase(s.begin());
	}
};

void RPStringRemoveTrailing(std::string& s, const std::string& cset) {
	if (s.empty() || cset.empty()) {return;}
	size_t idx = s.size() - 1;
	do {
		if (cset.find(s[idx]) == NPOS) {
			s = s.substr(0,idx + 1);
			return;
		}
		idx--;
	} while (idx > 0);
	if (cset.find(s[0]) == NPOS) { s = s[0]; }
	else { s = ""; }
	
};

void RPStringRemoveTrailing(std::string& s, const std::set<char>& cset) {
	if (s.empty() || cset.empty()) {return;}
	size_t idx = s.size() - 1;
	do {
		if (cset.find(s[idx]) == cset.end()) {
			s = s.substr(0,idx + 1);
			return;
		}
		idx--;
	} while (idx > 0);
	if (cset.find(s[0]) == cset.end()) { s = s[0]; }
	else { s = ""; }
};

void RPStringRemoveLeadingTrailing(std::string& s, const std::string& cset) {
	RPStringRemoveLeading(s, cset);
	RPStringRemoveTrailing(s, cset);
};

void RPStringRemoveLeadingTrailing(std::string& s, const std::set<char>& cset) {
	RPStringRemoveLeading(s, cset);
	RPStringRemoveTrailing(s, cset);
};

bool RPStringOnlyHasChars(const std::string& s, const std::string cset, bool invert) {
	if (s.empty()) {return invert;}
	if (cset.empty()) {return !invert;}
	if (!invert) { // only has cset characters
		for (size_t t=0; t < s.length(); t++) {
			if (cset.find(s[t]) == NPOS) {return false;}
		}
	}
	else {
		for (size_t t=0; t < s.length(); t++) {
			if (cset.find(s[t]) != NPOS) {return false;}
		}
	}
	return true;
};

bool RPStringOnlyHasChars(const std::string& s, const std::set<char> cset, bool invert) {
	if (s.empty()) {return invert;}
	if (cset.empty()) {return !invert;}
	if (!invert) { // only has cset characters
		for (size_t t=0; t < s.length(); t++) {
			if (cset.find(s[t]) == cset.end()) {return false;}
		}
	}
	else {
		for (size_t t=0; t < s.length(); t++) {
			if (cset.find(s[t]) != cset.end()) {return false;}
		}
	}
	return true;
};

long RPStringReplaceAll(std::string& origString, const std::string& toFind, const std::string& replacement) {
	if (origString.empty() || toFind.empty()) {return -1;}
	if (replacement.find(toFind) != NPOS) {return -2;}
	long ct=0;
	size_t pos=0;
	while ((pos = origString.find(toFind)) != NPOS) {
		origString.erase(origString.begin()+pos, origString.begin()+pos+toFind.length());
		origString.insert(pos, replacement);
		ct++;
	}
	return ct;
};

// replacement cannot be in charsToReplace
long RPStringReplaceAllChars(std::string& origString, const std::string& charsToReplace, char replacement) {
	if (charsToReplace.find(replacement) != NPOS) {return -1;}
	if (origString.empty() || charsToReplace.empty()) {return 0;}
	long ct=0;
	if (replacement) {
		for (size_t t=0; t < origString.length(); t++) {
			if (charsToReplace.find(origString[t]) != NPOS) {
				origString[t] = replacement;
				ct++;
			}
		}
	}
	else { // erase
		std::string::iterator itr;
		for (itr = origString.begin(); itr!= origString.end();) {
			if (charsToReplace.find(*itr) != NPOS) {
				itr = origString.erase(itr);
				ct++;
			}
			else {
				 itr++;
			}
		}
	}
	return ct;
};

long RPStringReplaceAllChars(std::string& origString, const std::set<char>& charsToReplace, char replacement) {
	if (charsToReplace.find(replacement) != charsToReplace.end()) {return -1;}
	if (origString.empty() || charsToReplace.empty()) {return 0;}
	long ct=0;
	if (replacement) {
		for (size_t t=0; t < origString.length(); t++) {
			if (charsToReplace.find(origString[t]) != charsToReplace.end()) {
				origString[t] = replacement;
				ct++;
			}
		}
	}
	else { // erase
		std::string::iterator itr;
		for (itr = origString.begin(); itr!= origString.end();) {
			if (charsToReplace.find(*itr) != charsToReplace.end()) {
				itr = origString.erase(itr);
				ct++;
			}
			else {
				 itr++;
			}
		}
	}
	return ct;
};

unsigned long RPStringCountOfChar(const std::string& s, char c) {
	if (s.empty() || c==0) {return 0;}
	unsigned long ct=0;
	for (size_t t=0; t<s.size(); t++) {
		if (s[t]==c) {ct++;}
	}
	return ct;
};

unsigned long RPLongestString(const StringArray& ary, size_t& index) {
	index = NPOS;
	unsigned long l = 0;
	for (size_t t=0; t<ary.size(); t++) {
		if (ary[t].length() > l) {l = ary[t].length();}
	}
	return l;
};

std::string RPStrFindFromBack(const std::string srce, char c) {
	if (srce.empty() || c==0) {return "";}
	size_t pos = srce.find_last_of(c);
	if (pos == NPOS) {
		return srce;
	}
	else {
		return srce.substr(pos+1);
	}
	
};

std::string RPStrBeforeMarker(const std::string srce, char c) {
	if (srce.empty() || c==0) {return "";}
	size_t pos = srce.find_last_of(c);
	if (pos == NPOS) {
		return "";
	}
	else {
		return srce.substr(0, pos);
	}
};

bool RPStringBeforeAndAfter(const std::string string, const std::string& divider, std::string& before, std::string& after) {
	size_t pos = string.find(divider);
	if (pos == NPOS) {return false;}
	if (pos == 0) {
		before="";
		after=string;
	}
	else if (pos == string.length()-1) {
		before = string;
		after = "";
	}
	else { // aa=bb
		before = string.substr(0, pos);
		after = string.substr(pos+1, string.length() - pos - 1);
	}
	return true;
};

std::string RPXByteString(unsigned long long bytes) {
	static StringArray suffix={"bytes", "KiB", "MiB", "GiB", "TiB", "PiB"};
	std::string s = "";
	int idx = 0;
	double v = bytes;
	
	while (v > 1023) {
		idx++;
		v /= 1024;
		if (idx == suffix.size()-1) {break;}
	}
	char buf[32]; bzero(buf,32);
	if (idx==0) {
		snprintf(buf, 31, "%.0f", v);
	}
	else {
		snprintf(buf, 31, "%.1f", v);
	}
	s = std::string(buf);
	s = s + " " + suffix[idx];
	
	return s;
};

std::string RPXByteString1000(unsigned long long bytes) {
	static StringArray suffix={"bytes", "KB", "MB", "GB", "TB", "PB"};
	std::string s = "";
	int idx = 0;
	double v = bytes;
	
	while (v > 999) {
		idx++;
		v /= 1000;
		if (idx == suffix.size()-1) {break;}
	}
	char buf[32]; bzero(buf,32);
	if (idx==0) {
		snprintf(buf, 31, "%.0f", v);
	}
	else {
		snprintf(buf, 31, "%.1f", v);
	}
	s = std::string(buf);
	s = s + " " + suffix[idx];
	
	return s;
};

unsigned long long RPStringToBytes(const std::string& byteString) {
	try {
		return RPStringToBytesEx(byteString);
	}
	catch (StringStuffEx& e) {
		printf("%s\n", e.what());
		return 0;
	};
};

unsigned long long RPStringToBytesEx(const std::string& byteString) {
	std::string line = byteString;
	RPStringReplaceAll(line, " ", "");
	if (RPStringCountOfChar(line, '.') > 1) { throw StringStuffEx("Too many decimal points"); }
	if (RPStringOnlyHasChars(line, "0123456789.", false)) {
		double d = std::strtod(line.c_str(), NULL);
		return (unsigned long long)d;
	}
	
	long long idx=0;
	std::string nums = "0123456789.";
	std::string numPart;
	std::string sfxPart;
	for (idx = 0; idx < line.size(); idx++) {
		if (nums.find_first_of(line[idx]) == std::string::npos) {
			numPart = line.substr(0, idx);
			sfxPart = line.substr(idx);
			break;
		}
	}
	
	double d = std::strtod(numPart.c_str(), NULL);
	RPStringLowercase(sfxPart);
	if (sfxPart == "pib") { d *= 1125899906842624; }
	else if (sfxPart == "pb") { d *= 1000000000000000; }
	else if (sfxPart == "tib") { d *= 1099511627776; }
	else if (sfxPart == "tb") { d *= 1000000000000; }
	else if (sfxPart == "gib") { d *= 1073741824; }
	else if (sfxPart == "gb") { d *= 1000000000; }
	else if (sfxPart == "mib") { d *= 1048576; }
	else if (sfxPart == "mb") { d *= 1000000; }
	else if (sfxPart == "kib") { d *= 1024; }
	else if (sfxPart == "kb") { d *= 1000; }
	else if (sfxPart == "byte" || sfxPart == "bytes") { ; }
	else { throw StringStuffEx("Invalid suffix"); }
	return (unsigned long long)d;
};

void RPOutputStringArray(int desc, const StringArray& ary, const std::string joiner) {
	
	std::string s="";
	StringArray::const_iterator itr;
	if (desc < 0) {
		for (auto itr = ary.begin(); itr!=ary.end(); itr++) {
			std::cout << *itr << joiner;
		}
	}
	else {
		// Could use for (auto strg : ary) {
		for (auto itr = ary.begin(); itr!=ary.end(); itr++) {
			dprintf(desc, "%s%s", (*itr).c_str(), joiner.c_str());
		}
	}
};

void RPOutputStringArray(std::ostream& stream, const StringArray& ary, const std::string joiner) {
	RPOutputSequence(stream, ary.begin(), ary.end(), joiner);
}

void RPOutputStringSet(int desc, const StringSet& set, const std::string joiner) {
	
	if (desc < 0) {
		for (auto itr = set.begin(); itr!=set.end(); itr++) {
			std::cout << *itr << joiner;
		}
	}
	else {
		for (auto itr = set.begin(); itr!=set.end(); itr++) {
			dprintf(desc, "%s%s", (*itr).c_str(), joiner.c_str());
		}
	}
};

void RPReduceConsecutiveChars(std::string& srce) {
	if (srce.empty()) {return;}
	char c=0, current=0;
	std::string::iterator itr;
	for (itr = srce.begin(); itr != srce.end();) {
		c = *itr;
		if (c != current) {current = c; itr++;}
		else {itr = srce.erase(itr);}
	}
};

void RPReduceConsecutiveChars(std::string& srce, char C) {
	if (srce.empty()) {return;}
	char c=0, current=0;
	std::string::iterator itr;
	for (itr = srce.begin(); itr != srce.end();) {
		c = *itr;
		if (c == current && c == C) {itr = srce.erase(itr);}
		else {current = c; itr++;}
	}
};

//-------------------------------------------------------------
#pragma mark -

template<>
void RPOutputNumVector(int desc, const std::vector<double>& array, const std::string joiner) {
	for (auto itr = array.begin(); itr != array.end(); itr++) {
		desc < 0 ? printf("%f%s", (*itr), joiner.c_str()) : dprintf(desc, "%f%s", (*itr), joiner.c_str());
	}
}

template<>
void RPOutputNumVector(int desc, const std::vector<float>& array, const std::string joiner) {
	for (auto itr = array.begin(); itr != array.end(); itr++) {
		desc < 0 ? printf("%f%s", (*itr), joiner.c_str()) : dprintf(desc, "%f%s", (*itr), joiner.c_str());
	}
}

}; // namespace
