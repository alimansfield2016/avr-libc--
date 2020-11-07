#pragma once

#include <type_traits>

namespace AVR
{
	template<class T>
	class eeprom_ptr
	{
	public:
		using value_type = std::remove_const<T>::type;
		using pointer = value_type*;
		using const_pointer = const value_type*;

	private:
		const_pointer m_ptr;

	public:
		constexpr eeprom_ptr(const T* _ptr) : m_ptr{_ptr}{}

		void erase(){}
		void write(T){}
	};
} // namespace AVR
