#ifndef AVR_INTERRUPT_INTERRUPT_HPP
#define AVR_INTERRUPT_INTERRUPT_HPP

#include <avr/io/io.hpp>
// #include <avr/interrupt.h>
#define M_EXPAND(...) __VA_ARGS__

#define USR_ISR2(vector, ...)                                                                  \
	extern "C" void __vector_##vector(void) __attribute__((signal, __INTR_ATTRS)) __VA_ARGS__; \
	void __vector_##vector(void) VEC_BODY(vector)

#define USR_ISR(vector, ...) USR_ISR2(vector, __VA_ARGS__)

// static AVR::STL::list<void (*)()> vector##_callbacks;

#define VECTORS_0()
#define VECTORS_1()       \
	USR_ISR(1, ISR_BLOCK) \
	VECTORS_0()
#define VECTORS_2()       \
	USR_ISR(2, ISR_BLOCK) \
	VECTORS_1()
#define VECTORS_3()       \
	USR_ISR(3, ISR_BLOCK) \
	VECTORS_2()
#define VECTORS_4()       \
	USR_ISR(4, ISR_BLOCK) \
	VECTORS_3()
#define VECTORS_5()       \
	USR_ISR(5, ISR_BLOCK) \
	VECTORS_4()
#define VECTORS_6()       \
	USR_ISR(6, ISR_BLOCK) \
	VECTORS_5()
#define VECTORS_7()       \
	USR_ISR(7, ISR_BLOCK) \
	VECTORS_6()
#define VECTORS_8()       \
	USR_ISR(8, ISR_BLOCK) \
	VECTORS_7()
#define VECTORS_9()       \
	USR_ISR(9, ISR_BLOCK) \
	VECTORS_8()
#define VECTORS_10()       \
	USR_ISR(10, ISR_BLOCK) \
	VECTORS_9()
#define VECTORS_11()       \
	USR_ISR(11, ISR_BLOCK) \
	VECTORS_10()
#define VECTORS_12()       \
	USR_ISR(12, ISR_BLOCK) \
	VECTORS_11()
#define VECTORS_13()       \
	USR_ISR(13, ISR_BLOCK) \
	VECTORS_12()
#define VECTORS_14()       \
	USR_ISR(14, ISR_BLOCK) \
	VECTORS_13()
#define VECTORS_15()       \
	USR_ISR(15, ISR_BLOCK) \
	VECTORS_14()
#define VECTORS_16()       \
	USR_ISR(16, ISR_BLOCK) \
	VECTORS_15()
#define VECTORS_17()       \
	USR_ISR(17, ISR_BLOCK) \
	VECTORS_16()
#define VECTORS_18()       \
	USR_ISR(18, ISR_BLOCK) \
	VECTORS_17()
#define VECTORS_19()       \
	USR_ISR(19, ISR_BLOCK) \
	VECTORS_18()
#define VECTORS_20()       \
	USR_ISR(20, ISR_BLOCK) \
	VECTORS_19()
#define VECTORS_21()       \
	USR_ISR(21, ISR_BLOCK) \
	VECTORS_20()
#define VECTORS_22()       \
	USR_ISR(22, ISR_BLOCK) \
	VECTORS_21()
#define VECTORS_23()       \
	USR_ISR(23, ISR_BLOCK) \
	VECTORS_22()
#define VECTORS_24()       \
	USR_ISR(24, ISR_BLOCK) \
	VECTORS_23()
#define VECTORS_25()       \
	USR_ISR(25, ISR_BLOCK) \
	VECTORS_24()
#define VECTORS_26()       \
	USR_ISR(26, ISR_BLOCK) \
	VECTORS_25()
#define VECTORS_27()       \
	USR_ISR(27, ISR_BLOCK) \
	VECTORS_26()
#define VECTORS_28()       \
	USR_ISR(28, ISR_BLOCK) \
	VECTORS_27()
#define VECTORS_29()       \
	USR_ISR(29, ISR_BLOCK) \
	VECTORS_28()
#define VECTORS_30()       \
	USR_ISR(30, ISR_BLOCK) \
	VECTORS_29()
#define VECTORS_31()       \
	USR_ISR(31, ISR_BLOCK) \
	VECTORS_30()
#define VECTORS_32()       \
	USR_ISR(32, ISR_BLOCK) \
	VECTORS_31()
#define VECTORS_33()       \
	USR_ISR(33, ISR_BLOCK) \
	VECTORS_32()
#define VECTORS_34()       \
	USR_ISR(34, ISR_BLOCK) \
	VECTORS_33()

#define DEFINE_VECTORS__(vec_count) \
	M_EXPAND(VECTORS_##vec_count)   \
	()
#define DEFINE_VECTORS_(count) DEFINE_VECTORS__(count)
#define DEFINE_VECTORS(count) DEFINE_VECTORS_(M_EXPAND(count))

// #define VEC_COUNT M_EXPAND(_VECTORS_SIZE / 4)
// #define VEC_COUNT 31

// DEFINE_VECTORS(VEC_COUNT)

namespace AVR
{

} // namespace AVR

#endif