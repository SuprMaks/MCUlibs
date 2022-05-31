#pragma once

#ifndef COMMON_H_
#define COMMON_H_

#include <avr/interrupt.h>
#include "atomic.h"
#include "Flag.h"

template <class T>
__attribute__((always_inline)) inline void DoNotOptimize(const T& value) {
	asm volatile("" : "+m"(const_cast<T&>(value)));
}

__attribute__((always_inline)) inline void memory(void) {
	asm volatile("" ::: "memory");
}

__attribute__((always_inline)) inline void nop(void) {
	asm volatile ("nop" ::);
}

namespace Private {
	template <class flag>
	class AtomicCovering {
		private:
			const bool tmp;
		public:
			inline AtomicCovering(void) : tmp(flag::isSet()) {
				flag::Clear();
			}
			inline ~AtomicCovering(void) {
				if (tmp) {
					flag::Set();
				}
			}
			inline operator bool() const {
				return false;
			}
	};

	/*template<size_t addr, const unsigned char b>
	class AtomicCovering<Flag<addr, b>>;*/

	/*template<>
	class AtomicCovering<Flag<Mcucpp::IoReg<0x3F>, SREG_I>> {
		private:
			const bool tmp;
		public:
			inline AtomicCovering(void) : tmp(Mcucpp::IoReg<0x3F>::BitIsSet<SREG_I>()) {
					cli();
			}
			inline ~AtomicCovering(void) {
				if (tmp) {
					sei();
				}
			}
			inline operator bool() const {
				return false;
			}
	};*/

	class EnableInterrupts {
	private:
		const bool tmp;
		typedef Flag<IOReg<0x3F + __SFR_OFFSET>, SREG_I> flag;

	public:
		inline __attribute__((always_inline)) EnableInterrupts(void) : tmp(flag::isSet()) {
			memory();
			flag::Set();
		}

		inline __attribute__((always_inline)) ~EnableInterrupts(void) {
			if (!tmp) {
				flag::Clear();
			}
		}

		inline __attribute__((always_inline)) operator bool() const {
			return false;
		}
	};

#ifdef PRR
	class EnablePowerReduction {
		private:
			const unsigned char _prr;
		public:
			EnablePowerReduction() : _prr(PRR) {
				PRR = 0xffu;
			}
			~EnablePowerReduction() {
				PRR = _prr;
			}
			operator bool() {
				return false;
			}
	};
#define DIS_AND_RESTORE_PRR if(Private::EnablePowerReduction di = Private::EnablePowerReduction()){}else
#endif // PRR

#define EN_AND_RESTORE_INT if(Private::EnableInterrupts di = Private::EnableInterrupts()){}else
#define ATOMIC_REG(flag) if(Private::AtomicCovering<flag> di = Private::AtomicCovering<flag>()){}else
}

constexpr const unsigned char reg_to_var(volatile uint8_t* addr) {
	return *(unsigned char*)&addr;
}

template<typename T, class flag = Flag<IOReg<0x3F + __SFR_OFFSET>, SREG_I>>
T atomic_read(T& var) {
	volatile T tmp;
	ATOMIC_REG(flag) {
		tmp = var;
	}
	return tmp;
}

template<const unsigned char addr = 0x3F + __SFR_OFFSET, const unsigned char b = SREG_I>
inline __attribute__((always_inline)) unsigned char atomic_read(unsigned char &var) {
	return var;
}

template<const unsigned char addr = 0x3F + __SFR_OFFSET, const unsigned char b = SREG_I>
inline __attribute__((always_inline)) signed char atomic_read(signed char &var) {
	return var;
}

template<const unsigned char addr = 0x3F + __SFR_OFFSET, const unsigned char b = SREG_I>
inline __attribute__((always_inline)) bool atomic_read(bool &var) {
	return var;
}

template <class T>
constexpr const T& max(const T& a, const T& b) {
	return a > b ? a : b;
}

template <class T>
constexpr const T& min(const T& a, const T& b) {
	return a < b ? a : b;
}

template<typename T1, typename T2, typename T3>
constexpr auto VOLT_DIVIDER(T1 u, T2  r1, T3  r2) { 
	return (double) (u * r2) / (r1 + r2);
}

template<typename T1, typename T2, typename T3>
constexpr auto VOLT_DIVIDER_REVERSE(T1 u, T2  r1, T3  r2) {
	return (double) (u * (r1 + r2)) / r2;
}

template<typename T>
constexpr T simple_round(T val) {
	return val < 0 ? val - 0.5 : val + 0.5;
}

template<typename T1, typename T2, typename T3>
constexpr auto V2ADC(T1 volt, T2  ref, T3  counts) {
	return (const unsigned int) simple_round(((double)volt * counts) / ref);
}

template<typename T1, typename T2, typename T3>
constexpr auto ADC2V(T1 adc, T2  ref, T3  counts) {
	return adc * ref / counts;
}

template<class T, size_t N>
constexpr size_t array_count(T(&array)[N]) {
	return N;
}

#endif