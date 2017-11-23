#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class Print {
	public:
		void print(std::string message, bool);
		Print();
	private: 
		std::mutex print_mutex;
		bool can_write;

};