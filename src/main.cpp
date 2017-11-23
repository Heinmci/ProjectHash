#include <iostream>
#include <queue>
#include <thread>
#include <cassert>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <cmath>
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
std::string get_time_message();
std::string get_word_message(std::string);
void found_match(std::string);

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
	for(int i = 1; i < 100; i++) {
		std::string word(i, '*');
		std::string message = "Testing word of length: " + std::to_string(i) + " | starting with letter: " + letters;
		print->print(message, true);
		for(int j = 0; j < 1; j++) {
			findPwd(word,i, letters, 0);
		} 
	}
}


void findPwd(std::string word, int max_depth, char letter, int current_depth) {
    if (current_depth == max_depth) {
    	bool is_match = matches_hash(word);
		if (is_match) {
			found_match(word);
		}
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

void found_match(std::string word) {
	std::string final_message = get_word_message(word) + get_time_message();
	print->print(final_message, false);
	exit(EXIT_SUCCESS);
}

std::string get_time_message() {
	double end = omp_get_wtime();
    double elapsed = end - begin;
    int minutes = (int)elapsed / 60;
    int seconds = (int)elapsed % 60;
    double test = (elapsed - floor(elapsed));
    int milliseconds = floor(test * 100);
    std::string message = "Time elapsed = " + std::to_string(minutes) + " minutes, " + std::to_string(seconds) + " seconds, " + std::to_string(milliseconds) + " milliseconds.";
    return message;
}

std::string get_word_message(std::string word) {
	return "Your secret word is: " + word + "\n"; 
}

bool matches_hash(std::string word_to_test) {
	std::string test_hash = sha256(word_to_test);

	if (argument_hash == test_hash) {
		return true;
	}

	return false;
}

