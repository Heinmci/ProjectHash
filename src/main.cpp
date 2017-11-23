#include <iostream>
#include <queue>
#include <thread>
#include <cassert>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "sha.h"
#include "print.h"

std::string argument_hash;
std::string dict = "abcdefghijklmnopqrstuvwxyz 0123456789";
double begin;
Print* print;

/* g++ -std=c++11 -pthread *.cpp*/
/* Need to lowercase the argument hash input*/

void findPwd(std::string,int, char, int);
bool matches_hash(std::string);
void getWordsOfDifferentLength(char, int);

int main( int argc, char *argv[] ) {
	print = new Print();

	argument_hash = argv[2];
	int nb_threads = atoi(argv[1]);

	std::thread myThreads[nb_threads];
	begin = omp_get_wtime();
	for(int i = 0; i < nb_threads; i++) {
        myThreads[i] = std::thread(&getWordsOfDifferentLength, dict.at(i), 1);
    }

    for(int i = 0; i < nb_threads; i++) {
        myThreads[i].join();
    }

    
}

void getWordsOfDifferentLength(char letters, int length) {
	//std::cout << letters << std::endl;
	for(int i = 1; i < 100; i++) {
		std::string word(i, '*');
		std::string message = "Testing word of length: " + std::to_string(i) + " | starting with letter: " + letters;
		print->print(message);
		//std::cout << "Testing word of length: " << i << " starting with letter:" << letters << std::endl;
		for(int j = 0; j < 1; j++) {
			//std::cout << word << std::endl;
			findPwd(word,i, letters, 0);
		} 
	}
}


void findPwd(std::string word, int max_depth, char letter, int current_depth) {
    if (current_depth == max_depth) {
    	//std::cout << word << std::endl;
    	bool is_match = matches_hash(word);
		if (is_match) {
			std::cout << "Your secret word is :" << word << std::endl; 
			double end = omp_get_wtime();
		    double elapsed = end - begin;
		    std::cout << "elapsed=" << elapsed << std::endl;
			exit(EXIT_SUCCESS);
		}
		//exit(EXIT_SUCCESS);
        return;
    }
    for (int i = 0; i < dict.length(); i++) {
    	if (current_depth == 0) {
    		word[0] = letter;
    		findPwd(word, max_depth, letter, current_depth + 1);
    		break;
    	} else {
    		word[current_depth] = dict[i];
        	findPwd(word, max_depth, letter, current_depth + 1);
    	}
    }
}

bool matches_hash(std::string word_to_test) {
	std::string test_hash = sha256(word_to_test);

	if (argument_hash == test_hash) {
		return true;
	}

	return false;
}

