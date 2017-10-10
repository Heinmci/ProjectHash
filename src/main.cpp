#include <iostream>
#include <queue>
#include <thread>
#include <cassert>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include "sha.h"

std::string argument_hash;
std::string dict = "abcdefghijklmnopqrstuvwxyz 0123456789";

void findPwd(std::string,int);
bool matches_hash(std::string);

int main( int argc, char *argv[] ) {

	argument_hash = argv[1];

	for(int i = 0; i < 100; i++) {
		std::string word(i, '*'); 
		findPwd(word,i);
	}
}

void findPwd(std::string word, int pos) {
    if (pos < 0) {
    	//std::cout << word << std::endl;
    	bool is_match = matches_hash(word);
		if (is_match) {
			std::cout << "Your secret word is :" << word << std::endl; 
			exit(EXIT_SUCCESS);
		}
        return;
    }
    for (int i = 0; i < dict.length(); i++) {
    	word[pos] = dict[i];
        findPwd(word, pos-1);
    }
}

bool matches_hash(std::string word_to_test) {
	std::string test_hash = sha256(word_to_test);
	//std::cout << test_hash << std::endl;
	if (argument_hash == test_hash) {
		return true;
	}

	return false;
}

