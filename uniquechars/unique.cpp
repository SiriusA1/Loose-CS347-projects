/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Conor McGullam
 * Date        : 2/15/2020
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
	//Uses ASCII values to make sure all letters are lowercase
    for(auto it = s.cbegin(); it != s.cend(); ++it) {
		if(*it < 97 || *it > 122) {
			return false;
		}
	}
	return true;
}

bool all_unique_letters(const string &s) {
	//bitset is used to store all letters that have already been seen
	unsigned int bitset = 0;
	//char_setter stores the char from the currently viewed index of the string
	unsigned char char_setter;
	//loop iterates through the string argument
	for(string::size_type i = 0; i < s.size(); ++i) {
		//subtracts ASCII value of 'a' from current char to get the "index" in bitset
		char_setter = s[i] - 'a';
		//left shift is done outside of the char_setter to avoid overflow when trying to store in an unsigned char
		/* If the '&' of bitset and leftshifted char_setter equals the leftshifted char_setter, then bitset already contains
		 * the current character. */
		if((bitset & (unsigned)(1<<char_setter)) == (unsigned)(1<<char_setter)) {
			return false;
		}else{
			//adding current char to the bitset
			bitset |= (unsigned)(1<<char_setter);
		}
	}
	return true;
}

int main(int argc, char * const argv[]) {
	if(argc != 2) {
		cout << "Usage: ./unique <string>" << endl;
	}else {
		string test = "";
		//turning char array into a string
		for(int i = 1; i < argc; ++i) {
			test += argv[i];
		}
	
		if(!is_all_lowercase(test)) {
			cout << "Error: String must contain only lowercase letters." << endl;
		}else if(all_unique_letters(test)) {
			cout << "All letters are unique." << endl;
		}else {
			cout << "Duplicate letters found." << endl;
		}
	}
}
