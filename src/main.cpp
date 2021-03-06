#include <iostream>
#include <queue>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <thread>
#include <cassert>
#include <mutex>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <omp.h>
#include <cmath>
#include <vector>

#include "print.h"

std::string argument_hash;
std::string dict = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789";
double begin;
Print* print;

/* g++ -std=c++11 -pthread *.cpp*/
/* Need to lowercase the argument hash input*/
std::string sha256(std::string);
void findPwd(std::string,int, char, int);
bool matches_hash(std::string);
void getWordsOfDifferentLength(std::vector<char>, int);
std::string get_word_message(std::string);
void found_match(std::string);
std::vector<int> get_nb_chars_per_thread(int);

int main( int argc, char *argv[] ) {
	print = new Print();

	argument_hash = argv[2];
	std::transform(argument_hash.begin(), argument_hash.end(), argument_hash.begin(), ::tolower);
	int nb_threads = atoi(argv[1]);

	std::thread myThreads[nb_threads];
	std::vector<int> nb_chars = get_nb_chars_per_thread(nb_threads);

	int current_index = 0;
	for(int i = 0; i < nb_threads; i++) {
		int nb_letters = nb_chars.at(i);
		std::vector<char> letters = {};
		for(int j = 0; j < nb_letters; j++) {
			letters.push_back(dict.at(current_index));
			current_index++;
		}
        myThreads[i] = std::thread(&getWordsOfDifferentLength, letters, 1);
    }

    for(int i = 0; i < nb_threads; i++) {
        myThreads[i].join();
    }

    
}

std::vector<int> get_nb_chars_per_thread(int nb_threads) {
	int dict_length = dict.length();

	int default_nb_of_letters = floor(dict_length / nb_threads);
	int remaining_letters = dict_length % nb_threads;

	std::vector<int> nb_chars = {};

	for(int i = 0; i < nb_threads; i++) {
		int nb_chars_for_thread = default_nb_of_letters;
		if (remaining_letters > 0) {
			nb_chars_for_thread += 1;
			remaining_letters -= 1;
		}
		nb_chars.push_back(nb_chars_for_thread);
	}
	return nb_chars;
}

void getWordsOfDifferentLength(std::vector<char> letters, int length) {
	for(int i = 1; i < 100; i++) {
		std::string word(i, '*');
		for(char letter: letters) {
			findPwd(word,i, letter, 0);
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
	std::string final_message = get_word_message(word);
	print->print(final_message, false);
	exit(EXIT_SUCCESS);
}

std::string get_word_message(std::string word) {
	return word; 
}

bool matches_hash(std::string word_to_test) {

	std::string test_hash = sha256(word_to_test);

	if (argument_hash == test_hash) {
		return true;
	}

	return false;
}

std::string sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}