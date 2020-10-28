#pragma once

#include <stdint.h>
#include <string>
#include <avr/interrupt/interrupt.hpp>
#include <avr/io/register.hpp>

#ifndef F_CPU
#error Serial requires F_CPU to be defined
#else
#if !F_CPU
#error Serial requires that F_CPU Must have a value
#endif
#endif

#include <avr/memory/pgmspace.hpp>

const char clr_str[] PROGMEM = "\033[2J";

namespace AVR
{

	class Serial
	{
	public:
		constexpr static AVR::pgm_ptr<char> clr = AVR::pgm_ptr<char>(clr_str);
		struct SerialRegisters
		{
			const AVR::Register8 r_udr;
			const AVR::Register16 r_ubrr;
			const AVR::Register8 r_ucsra;
			const AVR::Register8 r_ucsrb;
			const AVR::Register8 r_ucsrc;
		};

		struct SerialInterrupts
		{
			const uint8_t int_rx;
			const uint8_t int_udre;
			const uint8_t int_tx;
		};

		enum class Interrupt
		{
			rx,
			udre,
			tx,
		};


	private:
		const SerialRegisters m_regs;
		const SerialInterrupts m_ints;

		typedef volatile uint8_t reg_t;
		typedef volatile uint16_t wreg_t;


	private:
		inline constexpr static uint16_t calc_ubrr(uint32_t baud, bool u2x = true)
		{
			if (u2x)
				return F_CPU / (8 * baud) - 1;

			else
				return F_CPU / (16 * baud) - 1;
		}
		inline constexpr static uint16_t calc_baud(uint16_t ubrr, bool u2x = true)
		{
			if (u2x)
				return F_CPU / 8 * (ubrr + 1);
			else
				return F_CPU / 16 * (ubrr + 1);
		}

		inline void tx(char c) const __attribute__((always_inline))
		{
			while (!(m_regs.r_ucsra & 0x20))
				;
			m_regs.r_udr = c;
		}
		inline char rx() const __attribute__((always_inline))
		{
			while (!(m_regs.r_ucsra & 0x80))
				;
			return m_regs.r_udr;
		}

	public:
		inline void rx_enable()  const { m_regs.r_ucsrb |= 0x10; }
		inline void rx_disable() const { m_regs.r_ucsrb &= ~0x10; }
		inline void tx_enable()  const { m_regs.r_ucsrb |= 0x08; }
		inline void tx_disable() const { m_regs.r_ucsrb &= ~0x08; }

		inline void rxc_int_enable()  const { m_regs.r_ucsrb |= 0x80; }
		inline void rxc_int_disable() const { m_regs.r_ucsrb &= ~0x80; }
		inline void txc_int_enable()  const { m_regs.r_ucsrb |= 0x40; }
		inline void txc_int_disable() const { m_regs.r_ucsrb &= ~0x40; }
		inline void dre_int_enable()  const { m_regs.r_ucsrb |= 0x20; }
		inline void dre_int_disable() const { m_regs.r_ucsrb &= ~0x20; }

	public:
		constexpr Serial() = delete;
		constexpr Serial(const SerialRegisters &regs, const SerialInterrupts &ints)
			: m_regs(regs),
			  m_ints(ints)
		{
		}
		/**
		 * Open a serial connection using specified Baud Rate
		 * Optionally use 2x speed. 
		 * For speeds > 57600 Baud, 2x speed IS REQUIRED
		 * in order for the serial to work correctly
		 * Maximum speed with 2x is 115200 Baud
		 */
		inline void begin(uint32_t baud, bool u2x = true) const
		{
			uint16_t ubrr = calc_ubrr(baud, u2x);
			m_regs.r_ubrr = ubrr;
			if (u2x)
				m_regs.r_ucsra |= 0x02;
			else
				m_regs.r_ucsra &= ~0x02;

			m_regs.r_ucsrc = 0x06;

			//enable Rx AND Tx
			rx_enable();
			tx_enable();
			//Interrupts not currently enabled, but may be if required
		}
		inline uint32_t baud(bool u2x = true) const { return calc_baud(m_regs.r_ubrr, u2x); }

		inline void end() const
		{
			//disable all serial features
			rx_disable();
			tx_disable();
			//disable interrupts
			rxc_int_disable();
			txc_int_disable();
			dre_int_disable();
		}

		inline void registerHandler(void (*cb)(), Serial::Interrupt interrupt) const
		{
			uint8_t _int = 0;
			switch (interrupt)
			{
			case Serial::Interrupt::rx:
				_int = m_ints.int_rx;
				break;
			case Serial::Interrupt::udre:
				_int = m_ints.int_udre;
				break;
			case Serial::Interrupt::tx:
				_int = m_ints.int_tx;
				break;
			}
			if (_int == 0)
				return;
			AVR::Interrupt::registerHandler(cb, _int);
		}
		inline void unregisterHandler(void (*cb)(), Serial::Interrupt interrupt) const
		{
			uint8_t _int = 0;
			switch (interrupt)
			{
			case Serial::Interrupt::rx:
				_int = m_ints.int_rx;
				break;
			case Serial::Interrupt::udre:
				_int = m_ints.int_udre;
				break;
			case Serial::Interrupt::tx:
				_int = m_ints.int_tx;
				break;
			}
			if (_int == 0)
				return;
			AVR::Interrupt::unregisterHandler(cb, _int);
		}

		void PrintLn(const char *s = nullptr) const 
		{
			if (s)
				while (*s)
					tx(*s++);

			tx('\r');
			tx('\n');
		}

		void PrintLn(const std::fixed_string &str) const
		{
			const char *s = str.constData();
			if (s)
				while (*s){
					tx(*s);
					s++;
				}

			tx('\r');
			tx('\n');
		}

		/**
		 * Always inline this function to avoid issue
		 * where AVR crashes and resets weirdly.
		 * Not at all sure why this happens, 
		 * but think it might be a GCC issue
		 */
		inline void PrintLn(const AVR::pgm_ptr<char> &ps) const //__attribute__((always_inline))
		{
			const char *s = ps.ptr();
			char c;
			if(s)
				while((c=pgm_read_byte(s++)))
					tx(c);
			tx('\r');
			tx('\n');
		}

		inline void Print(const AVR::pgm_ptr<char> &ps) const //__attribute__((always_inline))
		{
			const char *s = ps.ptr();
			char c;
			if(s)
				while((c=pgm_read_byte(s++)))
					tx(c);
		}

		inline void Print(const char *s) const
		{
			if(s)
				while (*s)
					tx(*s++);
		}
		inline void Print(const std::fixed_string &str) const
		{
			Print('F');
			const char *s = str.constData();
			if (s)
				while (*s)
					tx(*s++);
		}
		inline void Print(char c) const
		{
			tx(c);
		}

		inline void PrintHex(uint8_t hex) const
		{
			uint8_t val = hex >> 4;
			if (val > 0x09)
				Print('A' + val - 0x0A);
			else
				Print('0' + val);

			val = hex & 0x0F;
			if (val > 0x09)
				Print('A' + val - 0x0A);
			else
				Print('0' + val);
		}

		inline char getChar() const { return rx(); }
		bool available() const { return m_regs.r_ucsra & 0x80; }
	};
	// inline constexpr static AVR::pgm_ptr<char> Serial::clr(clr_str);
} // namespace AVR
