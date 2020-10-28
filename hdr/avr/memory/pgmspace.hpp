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
	template<typename T>
	class pgm_ptr 
	{
	public:
		using pointer = const T*;

	private:
		pointer m_ptr;
		static constexpr short int s_size = sizeof(T);

	public:
		explicit constexpr pgm_ptr(pointer ptr) : m_ptr(ptr) {}
		constexpr pgm_ptr(const pgm_ptr &copy) : m_ptr(copy.m_ptr) {}

		constexpr operator bool() const { return m_ptr; }

		pgm_ptr &operator++() { m_ptr += s_size; return *this; }
		pgm_ptr operator++(int) { pointer cpy = m_ptr; m_ptr += s_size; return pgm_ptr(cpy); }
		pgm_ptr &operator--() { m_ptr -= s_size; return *this; }
		pgm_ptr operator--(int) { pointer cpy = m_ptr; m_ptr -= s_size; return pgm_ptr(cpy); }

		[[nodiscard]] const T operator*() const { 
			T t; 
			for(uint8_t i = 0; i < s_size; i++)
				static_cast<uint8_t*>(&t)[i] = pgm_read_byte(m_ptr+i); 
			return t; 
		}
/* 
		[[nodiscard]] const T operator*() const requires(s_size==1) { 
			T t; 
			static_cast<uint8_t>(t) = pgm_read_byte(m_ptr); 
			return t; 
		}
		[[nodiscard]] const T operator*() const requires(s_size==2) { 
			T t; 
			static_cast<uint8_t>(t) = pgm_read_word(m_ptr); 
			return t; 
		}
		[[nodiscard]] const T operator*() const requires(s_size==4) { 
			T t; 
			static_cast<uint8_t>(t) = pgm_read_dword(m_ptr); 
			return t; 
		} */
		// [[nodiscard]] const T operator*() const requires(std::isEqual) { 
		// 	T t; 
		// 	static_cast<uint8_t>(t) = pgm_read_dword(m_ptr); 
		// 	return t; 
		// }

		[[nodiscard]] const T operator[](int i) const { 
			return *pgm_ptr(m_ptr+s_size*i); 
		}

		void assign(const T &copy) {}
		pointer ptr() const { return m_ptr; }


	};
} // namespace AVR
