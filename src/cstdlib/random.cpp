#include <cstdlib>

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

Random Source

The source includes:
- rand
- srand

*/

unsigned long int rand_state = 0;
constexpr unsigned long int rand_X = 1234;
constexpr unsigned long int rand_Y = 5678;


int std::rand()
{
	// random number generator
	// currently a very basic 
	// (A*X + B) % N
	// generator
	rand_state *= rand_X;
	rand_state += rand_Y;

	// static_assert(false, "AVR::std::rand Unimplemented");

	return rand_state%RAND_MAX - RAND_MAX/2;

}

void std::srand(unsigned int seed)
{
	// random number generator seeder

	rand_state = seed | static_cast<unsigned long>(seed)<<16;

	// static_assert(false, "AVR::std::rand Unimplemented");

}