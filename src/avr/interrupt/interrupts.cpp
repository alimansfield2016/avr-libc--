/* #include <avr/interrupt/interrupt.hpp>

#include "./interrupts.hpp"

#include <array>
#include <list>

std::array<::std::list<void (*)()>, _VECTORS_COUNT + 1> AVR::Interrupt::s_callbacks;

#define VEC_BODY(vec)	{	AVR::Interrupt::Interrupt(vec);	}
// DEFINE_VECTORS(31)

using namespace AVR;

void Interrupt::Interrupt(uint8_t int_id)
{
	std::list<void (*)()> &cbs = AVR::Interrupt::s_callbacks[int_id]; 
	auto it = cbs.begin();                         
	auto end = cbs.end();                          
	for (; !(it == end); ++it)                        
		(*(*it))();                                
}


bool Interrupt::registerHandler(void (*cb)(), uint8_t vector)
{
	::std::list<void (*)()> &cbs = s_callbacks[vector];
	for (auto it = cbs.begin(); !(it == cbs.end()); ++it)
		if (*it == cb)
			return false;
	cbs.push_back(cb);
	return true;
}

bool Interrupt::unregisterHandler(void (*cb)(), uint8_t vector)
{
	::std::list<void (*)()> &cbs = s_callbacks[vector];
	for (auto it = cbs.begin(); !(it == cbs.end()); ++it)
		if (*it == cb)
		{
			cbs.erase(it);
			return true;
		}
	return false;
} */