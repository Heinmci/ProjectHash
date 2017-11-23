#include "print.h"

// Print::Print() : print_mutex() {

// }

// Print::~Print() {

// }

void Print::print(std::string message) {
	std::unique_lock<std::mutex> lock(print_mutex);
	std::cout << message << std::endl;
}
