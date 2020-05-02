#include "Log.h"

#include <iostream>

void Log::print(const char* status, const char* msg) {
	std::cout << status << ": " << msg << std::endl;
}

void Log::log(const char* msg) {
	print("LOG", msg);
}

void Log::err(const char* error) {
	print("ERROR", error);
}