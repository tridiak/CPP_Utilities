//
//  StringStuff.hpp
//  UtilityLib
//
//  Created by Mark Noble on 8/01/17.
//  Copyright © 2017 Mark Noble. All rights reserved.
//

#ifndef StringStuff_hpp
#define StringStuff_hpp

#include <stdio.h>
#ifdef __cplusplus
#include <functional>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <exception>
#include <ostream>

// For those who don't want to write 'StringStuff::'
#define SST StringStuff

namespace StringStuff {

typedef std::vector<std::string> StringArray;
typedef StringArray::iterator SAIterator;
typedef StringArray::const_iterator ConstSAIterator;
typedef std::set<std::string> StringSet;
typedef StringSet::iterator SSIterator;
typedef StringSet::const_iterator ConstSSIterator;

#define NPOS std::string::npos

//
struct StringStuffEx : std::exception {
	std::string reason;
	StringStuffEx(const std::string& r) { reason = r; }
	const char* _Nonnull what() const throw() { return reason.c_str(); }
};

/**
 Check to see if string toFind is in array ary.

 @param ary Array to check.
 @param toFind Strings to find.
 @param caseInsen true = case insensitive.
 @return true if present, false otherwise.
 */
bool RPStringInArray(const StringArray& ary, const std::string& toFind, bool caseInsen);

/**
 Check to see if any string in array toFind is in array ary.

 @param ary Array to check.
 @param toFind Strings to find.
 @param caseInsen true = csase insensitive.
 @return true if present, false otherwise.
 */
bool RPStringArrayInArray(const StringArray& ary, const StringArray& toFind, bool caseInsen);

/**
 Return string in-between 'open' and 'close'.
 If open is empty, return string before close.
 If close is empty, return string after open.
 If both are empty, return original string.
 If open or close are not present, an empty string is returned.

 @param s String to search.
 @param open String marking beginning of enclosing section. Can be empty.
 @param close String marking end of enclosing section. Can be empty.
 @param openPos Position of 'open'. Will be -1 if not present.
 @param closePos Position of 'close. This will always be > 'openPos' IF it exists. Will be -1 if not present.
 @return Sub-string of original string or empty.
 */
std::string RPStringBetween(const std::string& s, const std::string& open, const std::string& close, long& openPos, long& closePos);


/**
 Case insensitive string compare.
*/
bool RPStringEqualCaseInsen(const std::string& s, const std::string& r);

/**
Convert string to lower case.
*/
void RPStringLowercase(std::string& s);

/**
Convert string to upper case.
*/
void RPStringUpper(std::string& s);


/**
 Removes character 'c' if it is the last character.
 */
std::string RPRemoveIfLastChar(const std::string& s, char c);

/**
Returns last character of 's' or 0 if empty.
*/
char RPLastChar(const std::string& s);

/**
 Remove all characters in 'cset' from 's'.

 @param s String to search and modify.
 @param cset Characters to remove.
 @param invert if true, remove all characters NOT in 'cset'.
 */
void RPStringRemoveFromSet(std::string& s, const std::string& cset, bool invert);

/**
 Split string using divider 'divider'.

 @param s String to split
 @param divider Divider
 @return Array of split string. Only empty if s is empty. Otherwise always at least one element.
 */
StringArray RPStringSplit(const std::string& s, const std::string& divider);

/**
 Join array of strings into one string.

 @param sv Array to join.
 @param join Joiner between each string in array.
 @return New string.
 */
std::string RPStringJoin(const StringArray& sv, const std::string& join);

/**
 Remove all leading characters in 'cset' from 's'.
 */
void RPStringRemoveLeading(std::string& s, const std::string& cset);
void RPStringRemoveLeading(std::string& s, const std::set<char>& cset);

/**
 Remove all trailing characters in 'cset' from 's'.
 */
void RPStringRemoveTrailing(std::string& s, const std::string& cset);
void RPStringRemoveTrailing(std::string& s, const std::set<char>& cset);

/**
Remove all leading & trailing characters in 'cset' from 's'
*/
void RPStringRemoveLeadingTrailing(std::string& s, const std::string& cset);
void RPStringRemoveLeadingTrailing(std::string& s, const std::set<char>& cset);

/**
 Check and see if string only has specific characters.

 @param s String to check.
 @param cset Characters string must only have.
 @param invert if true, check if passed string does not have ANY of the characters in 'cset'.
 @return true if string only has characters in 'cset'. If 'invert' is true, true if string does not have any characters from 'cset'.
 */
bool RPStringOnlyHasChars(const std::string& s, const std::string cset, bool invert);
bool RPStringOnlyHasChars(const std::string& s, const std::set<char> cset, bool invert);

/**
 Replace all occurrences of 'toFind' with 'replacement'. Check is case insensitive.

 @param origString String to search and modify.
 @param toFind String to find.
 @param replacement Replacement string. Cam be empty.
 @return Number of times the replacement occurred. -1 if origString is empty. -2 if replacement = toFind.
 */
long RPStringReplaceAll(std::string& origString, const std::string& toFind, const std::string& replacement);

/**
 Replace any character in 'charsToReplace' with char 'replacement'.
 'charsToReplace' cannot contain 'replacement'.

 @param origString String to modify.
 @param charsToReplace Which characters to replace.
 @param replacement Replacement character. Can be empty -> deletion.
 @return Number of times the replacement occurred. -1 if 'charsToReplace' contains 'replacement'.
 */
long RPStringReplaceAllChars(std::string& origString, const std::string& charsToReplace, char replacement);
long RPStringReplaceAllChars(std::string& origString, const std::set<char>& charsToReplace, char replacement);

///Returns counts of char 'c' in 's'
unsigned long RPStringCountOfChar(const std::string& s, char c);

///Returns length of longest string in 'ary'. 'index' will be set to index of longest string in array
unsigned long RPLongestString(const StringArray& ary, size_t& index);

/**
 Find sub-string after last occurrence of char 'c' in 'srce'.
 Search starts from the end.

 @param srce String to search.
 @param c character to find.
 @return if srce is empty or c == 0, an empty string will be returned. If 'c' is not present, entire string will be returned.
 	Otherwise, sub-string afer c will be returned.
 */
std::string RPStrFindFromBack(const std::string srce, char c);

/**
 Find sub-string before LAST occurrence of char 'c' in 'srce'.
 Search starts from the end.

 @param srce String to search.
 @param c character to find.
 @return if srce is empty or c == 0, an empty string will be returned. If 'c' is not present, entire string will be returned.
 	Otherwise, sub-string before c will be returned.
 */
std::string RPStrBeforeMarker(const std::string srce, char c); // starts from back

/**
 Split string into two with the first occurrence of 'divider' being the cleave point.

 @param string String to search.
 @param divider Character divider.
 @param before First half of string provided 'divider' exists. Undefined otherwise.
 @param after Second half of string provided 'divider' exists. Undefined otherwise.
 @return true if split is successful. Otherwise false.
 */
bool RPStringBeforeAndAfter(const std::string string, const std::string& divider, std::string& before, std::string& after);

///Convert passed integer to XXX byte/KiB/MiB/GiB/TiB/PiB in multiplies of 1024
std::string RPXByteString(unsigned long long bytes); // 1024

///Convert passed integer to XXX byte/KB/MB/GB/TB/PB in multiplies of 1000
std::string RPXByteString1000(unsigned long long bytes); // 1000

///String to ULL with magnitude suffix. Recognised suffixes: byte/KiB/MiB/GiB/TiB/PiB/KB/MB/GB/TB/PB.
/// XXX.YYY <suffix>. If the byteString contains invalid format or characters, zero will be returned.
unsigned long long RPStringToBytes(const std::string& byteString);

///String to ULL with magnitude suffix. Recognised suffixes: byte/KiB/MiB/GiB/TiB/PiB/KB/MB/GB/TB/PB.
/// XXX.YYY <suffix>. If the byteString contains invalid format or characters, a StringStuffEx exception will be thrown.
unsigned long long RPStringToBytesEx(const std::string& byteString);

/**
 Write contents of array to passed descriptor using optional joiner.

 @param desc posix descriptor. If < 0, printf() will be used.
 @param ary Array to output.
 @param joiner Joiner
 */
void RPOutputStringArray(int desc, const StringArray& ary, const std::string joiner="\n");

/**
 Write contents of set to passed descriptor using optional joiner.

 @param desc posix descriptor. If < 0, printf() will be used.
 @param set Set to output.
 @param joiner Joiner.
 */
void RPOutputStringSet(int desc, const StringSet& set, const std::string joiner="\n");

/**
Write range [begin,end] to output stream with a joiner.
*/
template <class InputIterator>
void RPOutputSequence(std::ostream& stream, InputIterator begin, InputIterator end, const std::string joiner="\n");

///Sequences of characters are reduced to a single character. e.g. ABCCCDDE -> ABCDE
void RPReduceConsecutiveChars(std::string& srce);

///Convert sequences of character 'c' from 'srce' into a single character 'c'.
///e.g. c = '1'. A111BCD11E -> A1BCD1E
void RPReduceConsecutiveChars(std::string& srce, char c);

//--------------------------------------------------------
#pragma mark -

// Write sequence to stream withg joiner
template <class InputIterator>
void RPOutputSequence(std::ostream& stream, InputIterator begin, InputIterator end, const std::string joiner) {
	while (begin != end) {
		auto v = *begin;
		stream << v << joiner;
		begin++;
	}
}

/**
Write numeric array to descriptor.
Underlying type converted to unsigned long long.
*/
template <class T>
void RPOutputNumVector(int desc, const std::vector<T>& array, const std::string joiner) {
	for (auto itr = array.begin(); itr != array.end(); itr++) {
		desc < 0 ? printf("%llu%s", (unsigned long long)(*itr), joiner.c_str()) : dprintf(desc, "%llu%s", (unsigned long long)(*itr), joiner.c_str());
	}
}

// double specialisation
template<>
void RPOutputNumVector(int desc, const std::vector<double>& array, const std::string joiner);

// float specialisation
template<>
void RPOutputNumVector(int desc, const std::vector<float>& array, const std::string joiner);

}; // namespace StringStuff

#endif // C++


#endif /* StringStuff_hpp */
