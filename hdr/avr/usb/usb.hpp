#pragma once

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

AVR USB Header

The <avr/usb/usb.hpp> header includes:
- USB device
- USB Interface
- USB endpoints

*/

namespace AVR
{
	namespace USB
	{
		using callback = void();
		/**initialise the hardware for USB
		 * This includes setting the appropriate port directionality
		 * and pullup resistors
		 */		
		extern void init();
		extern callback *on_reset;
	} // namespace USB
	
} // namespace AVR
