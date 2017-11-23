#include "print.h"

Print::Print() : print_mutex() {
	this->can_write = true;
}

// Print::~Print() {

// }

void Print::print(std::string message, bool can_write) {
	if (!this->can_write) {
		return;
	}
	std::unique_lock<std::mutex> lock(print_mutex);
	this->can_write = can_write;
	std::cout << message << std::endl;
}
