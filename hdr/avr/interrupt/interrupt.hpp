#ifndef AVR_INTERNAL_INTERRUPTS_HPP
#define AVR_INTERNAL_INTERRUPTS_HPP

#ifdef ENABLE_INTERRUPTS
#error
#include <avr/interrupt/interrupts.hpp>
#endif

// #include <avr/interrupt.h>
#include <avr/io/io.hpp>
#include <array>
#include <list>

#define VEC_BODY(vec)	{	AVR::Interrupt::Interrupt(int_id);	}

namespace AVR
{

	namespace Interrupt
	{

		static ::std::array<::std::list<void (*)()>, _VECTORS_COUNT + 1> s_callbacks;

		[[maybe_unused]] static void Interrupt(uint8_t int_id)
		{
		::std::list<void (*)()> &cbs = s_callbacks[int_id]; 
		auto it = cbs.begin();                         
		auto end = cbs.end();                          
		for (; !(it == end); ++it)                        
			(*(*it))();                                
		}


		inline static void enable() { __asm__ __volatile__ ("sei" ::: "memory"); }
		inline static void disable() { __asm__ __volatile__ ("cli" ::: "memory"); }

		inline static bool registerHandler(void (*cb)(), uint8_t vector)
		{
			::std::list<void (*)()> &cbs = s_callbacks[vector];
			for (auto it = cbs.begin(); !(it == cbs.end()); ++it)
				if (*it == cb)
					return false;
			cbs.push_back(cb);
			return true;
		}

		inline static bool unregisterHandler(void (*cb)(), uint8_t vector)
		{
			::std::list<void (*)()> &cbs = s_callbacks[vector];
			for (auto it = cbs.begin(); !(it == cbs.end()); ++it)
				if (*it == cb)
				{
					cbs.erase(it);
					return true;
				}
			return false;
		}
	}
	namespace Interrupt
	{

		class InterruptKeeper
		{
		private:
			bool prev_state;

		public:
			InterruptKeeper()
			{
				prev_state = SREG & _BV(SREG_I);
			}
			~InterruptKeeper()
			{
				if (prev_state)
					enable();
				else
					disable();
			}

		};

		class InterruptHolder : public InterruptKeeper
		{
		public:
			InterruptHolder() : InterruptKeeper() 
			{
				disable();
			}
		};

		class InterruptWindow : public InterruptKeeper
		{
		public:
			InterruptWindow() : InterruptKeeper()
			{
				enable();
			}
		};


		// DEFINE_VECTORS(_VECTORS_COUNT)


	} // namespace Interrupt

} // namespace AVR

#endif