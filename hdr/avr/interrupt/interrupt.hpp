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


namespace AVR
{

	namespace Interrupt
	{
		// #ifdef ENABLE_INTERRUPTS
		extern ::std::array<::std::list<void (*)()>, _VECTORS_COUNT + 1> s_callbacks;
		
		[[maybe_unused]] void Interrupt(uint8_t int_id);
		bool registerHandler(void (*cb)(), uint8_t vector);
		bool unregisterHandler(void (*cb)(), uint8_t vector);
		// #endif


		inline static void enable() { __asm__ __volatile__ ("sei" ::: "memory"); }
		inline static void disable() { __asm__ __volatile__ ("cli" ::: "memory"); }

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