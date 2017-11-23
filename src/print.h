#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class Print {
	public:
		void print(std::string message);
	private: 
		std::mutex print_mutex;

};