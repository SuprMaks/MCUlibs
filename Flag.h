#pragma once

#include <avr/io.h>
#include "IOReg.h"

template<class Reg, const unsigned char b> struct Flag;

template<const unsigned char addr, const unsigned char b>
struct Flag<IOReg<addr>, b> {
	inline __attribute__((always_inline)) static bool isSet(void) {
		return IOReg<addr>::bit::template read<b>();
	}

	inline __attribute__((always_inline)) static void Set(void) {
		IOReg<addr>::bit::template set<b>();
	}

	inline __attribute__((always_inline)) static void Clear(void) {
		IOReg<addr>::bit::template clear<b>();
	}
};

template<>
struct Flag<IOReg<0x3F + __SFR_OFFSET>, SREG_I> {
	inline __attribute__((always_inline)) static bool isSet(void) {
		return IOReg<0x3F + __SFR_OFFSET>::bit::read<SREG_I>();
	}

	inline __attribute__((always_inline)) static void Set(void) {
		sei();
	}

	inline __attribute__((always_inline)) static void Clear(void) {
		cli();
	}
};