#pragma once

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Vector.h"
#include "BinaryOpr.h"

#ifdef TIMER0_COMPA_vect
	ISRv(TIMER0_COMPA_vect)
#endif

#ifdef TIMER0_COMPB_vect
	ISRv(TIMER0_COMPB_vect)
#endif

#ifdef INT0_vect
	ISRv(INT0_vect)
#endif

template<class TX_pin>
class Soft_uart_tx {
	protected:
		#ifdef TIMER0_COMPA_vect
			friend ISRf(TIMER0_COMPA_vect);
		#endif
		#ifdef TIMER0_COMPB_vect
			friend ISRf(TIMER0_COMPB_vect);
		#endif
		volatile unsigned short int txbyte;
		volatile unsigned char txbitcount;

		void inline timer_callback(void) {
			TX_pin::Set(txbyte & 0x01);
			txbyte = (txbyte >> 0x01) + 0x8000;
			if (txbitcount > 0) {
				txbitcount--;
			}
		}

	public:
		// BAUD_DIV = (CPU_CLOCK / DIV) / BAUD_RATE
		void inline init(const unsigned char div, const unsigned int baud_div) {
			TX_pin::SetDirWrite();
			TX_pin::Set();
			OCR0A = baud_div;
			bitSet(TIMSK0, OCIE0A);
			bitSet(TCCR0A, WGM01);
			bitmask_Set(TCCR0B, div);
			sei();
		}

		// BAUD_DIV = (CPU_CLOCK / DIV) / BAUD_RATE
		Soft_uart_tx(void): txbyte(0xFFFF), txbitcount(0u) {
			//init(div, baud_div);
		}

		//template<typename T> void operator()(T tp);

		void operator()(unsigned char tb) {
			while (txbitcount);
			txbyte = (tb + 0xFF00) << 0x01;
			txbitcount = 0x0A;
		}

		void operator()(unsigned short int tb) {
			operator()((unsigned char)(tb >> 8));
			operator()((unsigned char)tb);
		}

		void operator()(char * str) {
			while (*str) {
				operator()(*str++);
			}
		}
};

template<class RX_pin>
class Soft_uart_rx {
	protected:
		#ifdef WDT_vect
			friend ISRf(INT0_vect);
		#endif
		#ifdef WDT_vect
			friend ISRf(TIMER0_COMPB_vect);
		#endif

		const unsigned int baud_div;
		unsigned char rxbyte;
		unsigned char rxbitcount;

		void timer_callback(void) {
			if (RX_pin::IsSet()) {
				rxbyte |= 0x80;
			}

			if (--rxbitcount == 0) {
				TIMSK0 &= ~(1 << OCIE0B);
				TIFR0 |= (1 << OCF0B);
				GIFR |= (1 << INTF0);
				GIMSK |= (1 << INT0);
			}
			else {
				rxbyte >>= 0x01;
			}
		}

		void int_callback(void) {
			rxbitcount = 0x09;
			rxbyte = 0x00;
			constexpr unsigned int baud_h_div = baud_div / 2;
			if (TCNT0 < baud_h_div) {
				OCR0B = TCNT0 + baud_h_div;
			}
			else {
				OCR0B = TCNT0 - baud_h_div;
			}
			GIMSK &= ~(1 << INT0);
			TIFR0 |= (1 << OCF0A) | (1 << OCF0B);
			TIMSK0 |= (1 << OCIE0B);
		}

	public:
		Soft_uart_rx(const unsigned int baud_d): rxbyte(0u), rxbitcount(9u), baud_div(baud_d) {
			RX_pin::SetDirRead();
			RX_pin::SetPullUp(RX_pin::Port::PullUp);
			MCUCR |= (1 << ISC01);
			GIMSK |= (1 << INT0);
			sei();
		}
};

#endif