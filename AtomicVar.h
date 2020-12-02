#pragma once
#include <avr/io.h>
#include "Common.h"

template<typename T, const unsigned char addr = 0x3F, const unsigned char b = SREG_I>
class AtomicVar {
	private:
		T val;
	public:
		AtomicVar(T v = 0) : val(v) {}

		T operator=(T var) {
			volatile T tmp;
			ATOMIC_REG(addr, b) {
				val = var;
				tmp = val;
			}
			return tmp;
		}
}

template<const unsigned char addr = 0x3F, const unsigned char b = SREG_I>
class AtomicVar<bool, addr, b> {
private:
	bool val;
public:
	AtomicVar(T v = 0) : val(v) {}

	bool operator=(bool var) {
			val = var;
		return val;
	}
}

template<const unsigned char addr = 0x3F, const unsigned char b = SREG_I>
class AtomicVar<unsigned char, addr, b> {
private:
	unsigned char val;
public:
	AtomicVar(T v = 0) : val(v) {}

	unsigned char operator=(unsigned char var) {
		val = var;
		return val;
	}
}

template<const unsigned char addr = 0x3F, const unsigned char b = SREG_I>
class AtomicVar<signed char, addr, b> {
private:
	unsigned char val;
public:
	AtomicVar(T v = 0) : val(v) {}

	signed char operator=(signed char var) {
		val = var;
		return val;
	}
}