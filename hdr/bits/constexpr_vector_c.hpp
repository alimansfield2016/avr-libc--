#pragma once

#include <initializer_list>
#include <type_traits>
#include "constexpr_vector.hpp"

namespace std
{
	template<class T>
	class constexpr_vector;
	
	template<class T, size_t S>
	class constexpr_vector_c : public constexpr_vector<T>
	{
		friend class constexpr_vector<T>;
		using value_type = std::remove_const<T>::type;
		value_type m_arr[S];

	public:
		constexpr constexpr_vector_c(const std::initializer_list<T> &init) : 
			constexpr_vector<T>{m_arr, S}
			{
				auto it = init.begin();
				for(uint8_t i = 0; i < S && it != init.end(); i++)
					m_arr[i] = *it++;
			}

	};

	template<class T>
	class constexpr_vector_c<T, 0> : public constexpr_vector<T>
	{
	public:
		constexpr constexpr_vector_c(const std::initializer_list<T> &) : 
			constexpr_vector<T>{nullptr, 0}{}
	};

} // namespace std
