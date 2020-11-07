#pragma once

/*
AVR C++ Standard Library
Written by Alex Mansfield
Copyright 2020

Program Space Header

The <avr/memory/pgmspace.hpp> header includes:
- pgm_ptr


*/

//Disable include of avr/io.h
#ifndef _AVR_IO_H_
#	define _AVR_IO_H_
#	include <avr/pgmspace.h>
#	undef _AVR_IO_H_
#else
#	include <avr/pgmspace.h>
#endif

#include <type_traits>


namespace AVR
{

	template<class T> //requires(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4)
	class pgm_ptr;

	/**AVR::pgm_anchor
	 * Similarly to the pgm_ptr, 
	 * the pgm_anchor is used to access data in PROGMEM.
	 * HOWEVER, There is one key difference.
	 * pgm_ptr is for a pointer ALREADY in PROGMEM
	 */

	class _pgm_ptr
	{
	protected:
		_pgm_ptr() = default;
		static void get(const uint8_t *ptr, uint8_t *buf, uint16_t size)
		{
			for(uint8_t i = 0; i < size; i++)
				buf[i] = pgm_read_byte(ptr++);
		}
	};

	template<typename T>
	class pgm_anchor
	{
		//we know that we ourselves are in Program Memory
		//so we should handle that too.

	public:
		using value_type = std::remove_const<T>::type;
		using const_value_type = const value_type;
		using pointer = value_type*;
		using const_pointer = const value_type*;

	private:
		const_pointer m_ptr;
		static constexpr short int s_size = sizeof(T);

		//when using any member variables, we MUST dereference their address

	public:
		constexpr pgm_anchor(const T* ptr) : m_ptr{ptr} {}
		pgm_anchor(pgm_anchor&) = default; //delete copy constructor
		pointer ptr() const { return *pgm_ptr(&m_ptr); }

		constexpr operator bool() const { return ptr(); }
		[[nodiscard]] T operator*() const {
			return *pgm_ptr{ptr()};
		}
		[[nodiscard]] T operator[](int i) const { 
			return *pgm_ptr{&(ptr()[i])}; 
		}

	};

	/**AVR::pgm_ptr
	 * Pointer to data in ProgMem
	 * Data types used MUST have a sizeof 1 OR 2 OR 4.
	 * Any other sizes WILL NOT WORK
	 * This is generally used for storage of trivial types 
	 * such as numbers or buffers
	 * 
	 */
	template<typename T> //requires(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4)
	class pgm_ptr : protected _pgm_ptr
	{
	public:
		using value_type = std::remove_const<T>::type;
		using const_value_type = const value_type;
		using pointer = value_type*;
		using const_pointer = const value_type*;

	private:
		const_pointer m_ptr;
		static constexpr short int s_size = sizeof(T);

	public:
		constexpr pgm_ptr(const T* ptr = nullptr) : m_ptr(ptr) {}
		constexpr pgm_ptr(const pgm_ptr &copy) : m_ptr(copy.m_ptr) {}
		constexpr pgm_ptr(const pgm_anchor<value_type> &copy) : m_ptr{copy.ptr()} {}
		constexpr const_pointer ptr() const { return m_ptr; }
		void assign(const_pointer ptr) { m_ptr = ptr; }

		constexpr pgm_ptr& operator=(const pgm_ptr &copy) { m_ptr = copy.m_ptr; return *this; }

		constexpr operator bool() const { return m_ptr; }

		constexpr pgm_ptr &operator++() { m_ptr += s_size; return *this; }
		constexpr pgm_ptr operator++(int) { const_pointer cpy = m_ptr; m_ptr += s_size; return pgm_ptr(cpy); }
		constexpr pgm_ptr &operator--() { m_ptr -= s_size; return *this; }
		constexpr pgm_ptr operator--(int) { const_pointer cpy = m_ptr; m_ptr -= s_size; return pgm_ptr(cpy); }

		constexpr value_type operator*() const
		{
			const uint8_t* p = reinterpret_cast<const uint8_t*>(ptr());
			// const uint8_t* p = reinterpret_cast<const uint8_t*>(m_ptr);
			uint8_t buf[s_size];
			for(uint8_t i = 0; i < s_size; i++)
				buf[i] = pgm_read_byte(p++);
			// get(p, buf, s_size);
			return *reinterpret_cast<pointer>(&buf[0]);
		}

		constexpr value_type operator[](int i) const { 
			return *pgm_ptr{&m_ptr[i]}; 
		}



	};

} // namespace AVR
