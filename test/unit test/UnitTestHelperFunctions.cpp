#include "UnitTestHelperFunctions.hpp"

#include <iostream>

void TEST_COUT(const char* pMessage)
{
	// Color code:
	//	        foreground background
	// black        30         40
	// red          31         41
	// green        32         42
	// yellow       33         43
	// blue         34         44
	// magenta      35         45
	// cyan         36         46
	// white        37         47
	std::cout << "\033[1;33m[MESSAGE   ]\033[0m" << "\033[1;33m " << pMessage << "\033[0m" << std::endl;
}
