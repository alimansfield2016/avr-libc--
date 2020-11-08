#pragma once

#include <type_traits>

#include "constexpr_vector_c.hpp"

namespace std
{
	
	template<class T>
	class constexpr_vector
	{
	public:
		// using value_type = T;
		using value_type = std::remove_const<T>::type;
		using pointer = value_type*;
		using const_pointer = const T*;
		using iterator = pointer;
		using const_iterator = const T*;
		using reference = value_type&;
		using const_reference = const T&;
		using size_type = size_t;
	private:
		const size_type m_size;
		
		

	public:
		constexpr constexpr_vector(size_type _size) : 
			m_size{_size} {}

		constexpr_vector(constexpr_vector&) = delete;
		constexpr_vector(const constexpr_vector&) = delete;
		constexpr_vector(constexpr_vector&&) = delete;
		constexpr_vector(const constexpr_vector&&) = delete;
			
		constexpr pointer ptr() { return static_cast<constexpr_vector_c<value_type, 1>*>(this)->m_arr; }
		constexpr const_pointer ptr() const { return static_cast<const constexpr_vector_c<value_type, 1>*>(this)->m_arr; }

		constexpr iterator begin() { return ptr(); }
		constexpr iterator end() { return &ptr()[m_size]; }
		constexpr const_iterator begin() const { return ptr(); }
		constexpr const_iterator end() const { return &ptr()[m_size]; }

		constexpr const_iterator cbegin() const { return ptr(); }
		constexpr const_iterator cend() const { return ptr(); }

		constexpr size_type size() const { return m_size; }
		constexpr const size_type* size_p() const { return &m_size; }

		constexpr reference operator[](size_type idx) { return ptr()[idx]; }
		constexpr const_reference operator[](size_type idx) const { return ptr()[idx]; }

	};
} // namespace std
