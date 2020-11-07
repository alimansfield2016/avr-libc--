#pragma once

#include <stdint.h>

namespace AVR
{
	template<typename Width>
	class Register{
		public:
			using T = Width;
			using VT = volatile T;
			explicit constexpr Register(uintptr_t addr) : m_addr(addr){};

			constexpr VT& reg() const { return *reinterpret_cast<VT*>(m_addr); }

			operator T() const {return reg(); }
			constexpr T operator = (T other) const { return reg() = other; }
			constexpr T operator |= (T other) const { return reg() |= other; }
			constexpr T operator &= (T other) const { return reg() &= other; }
			constexpr T operator ^= (T other) const { return reg() ^= other; }
			constexpr uintptr_t operator & () const { return m_addr; }
			constexpr uintptr_t io_addr() const { return m_addr-0x20; }
		protected:
		private:
		const uintptr_t m_addr;
	};
	using Register8 = Register<uint8_t>;
	using Register16 = Register<uint16_t>;

	template<typename Width> 
	class Bitmask
	{
		public:
		using T = Width;
		explicit constexpr Bitmask(uint8_t bit) : m_mask(1<<bit){};

		constexpr operator T() const { return m_mask; }

		constexpr Bitmask operator | (const Bitmask &other) const { return Bitmask(m_mask|other.m_mask, true); }
		constexpr Bitmask operator & (const Bitmask &other) const { return Bitmask(m_mask&other.m_mask, true); }

		private:
		constexpr Bitmask(T value, bool) : m_mask(value){};
		const T m_mask;

	};

	using BV = Bitmask<uint8_t>;
	using BV8 = BV;
	using BV16 = Bitmask<uint16_t>;
} // namespace AVR
