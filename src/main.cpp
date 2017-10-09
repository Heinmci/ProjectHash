#include <iostream>
#include <queue>
#include <thread>
#include <cassert>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include "sha.h"

int main( int argc, char *argv[] ) {

	std::string hash_to_decode = argv[1];
	int found = false;
	char buffer [33];

	for(int i = 97; i < 123; i++) {
		char bob = i;
		std::string s(1, bob);
		//std::cout << bob << std::endl;
		std::string test_hash = sha256(s);
		//std::cout << test_hash << std::endl;
		if (hash_to_decode == test_hash) {
			std::cout << "Your secret word is :" << s << std::endl; 
			found = true;
		}

	}

	if (!found) {
		std::cout << "Didnt find secret word"<<std::endl;
	}
	
}