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
	template<typename T>
	class pgm_anchor
	{
		//we know that we ourselves are in Program Memory
		//so we should handle that too.

	public:
		using type = const T;
		using pointer = type*;

	private:
		pointer p_ptr;
		static constexpr short int s_size = sizeof(T);

		//when using any member variables, we MUST dereference their address
		pointer m_ptr() const { return *pgm_ptr(&p_ptr); }

	public:
		explicit constexpr pgm_anchor(pointer ptr) : p_ptr{ptr} {}
		pgm_anchor(pgm_anchor&) = delete; //delete copy constructor

		constexpr operator bool() const { return m_ptr(); }

		// [[nodiscard]] const T operator*() const requires(s_size==1) { 
		// 	T t; 
		// 	*reinterpret_cast<uint8_t*>(&t) = pgm_read_byte(m_ptr()); 
		// 	return t; 
		// }
		// [[nodiscard]] const T operator*() const requires(s_size==2) { 
		// 	T t; 
		// 	*reinterpret_cast<uint16_t*>(&t) = pgm_read_word(m_ptr()); 
		// 	return t; 
		// }
		// [[nodiscard]] const T operator*() const requires(s_size==4) { 
		// 	T t; 
		// 	*reinterpret_cast<uint32_t*>(&t) = pgm_read_dword(m_ptr()); 
		// 	return t; 
		// } 


		[[nodiscard]] const T operator[](int i) const { 
			return *pgm_ptr{m_ptr()+s_size*i}; 
		}
		pointer ptr() const { return m_ptr(); }

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
	class pgm_ptr 
	{
	public:
		using value_type = T;
		using pointer = value_type*;

	private:
		const T* m_ptr;
		static constexpr short int s_size = sizeof(T);

	public:
		explicit constexpr pgm_ptr(const T* ptr = nullptr) : m_ptr(ptr) {}
		constexpr pgm_ptr(const pgm_ptr &copy) : m_ptr(copy.m_ptr) {}
		constexpr pgm_ptr(const pgm_anchor<T> &copy) : m_ptr{copy.m_ptr()} {}

		constexpr pgm_ptr& operator=(const pgm_ptr &copy) { m_ptr = copy.m_ptr; return *this; }

		constexpr operator bool() const { return m_ptr; }

		constexpr pgm_ptr &operator++() { m_ptr += s_size; return *this; }
		constexpr pgm_ptr operator++(int) { const T* cpy = m_ptr; m_ptr += s_size; return pgm_ptr(cpy); }
		constexpr pgm_ptr &operator--() { m_ptr -= s_size; return *this; }
		constexpr pgm_ptr operator--(int) { const T* cpy = m_ptr; m_ptr -= s_size; return pgm_ptr(cpy); }

		constexpr T operator*() const
		{
			const uint8_t *ptr = reinterpret_cast<const uint8_t*>(m_ptr);
			uint8_t b[s_size];
			for(uint8_t i = 0; i < s_size; i++){
				b[i] = pgm_read_byte(ptr++);
			}
			return *reinterpret_cast<T*>(&b[0]);
		}

		constexpr T operator[](int i) const { 
			return *pgm_ptr{&m_ptr[i]}; 
		}

		void assign(const T* ptr) { m_ptr = ptr; }
		const T* ptr() const { return m_ptr; }


	};

} // namespace AVR
