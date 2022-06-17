#pragma once

#ifndef BUTTON_H_
#define BUTTON_H_

#include "SimpleSensor.h"
#include "Vector.h"
#include <avr/io.h>
#include "Common.h"
#include "Callback.h"
#include "atomic.h"

#ifdef PCINT0_vect
	ISRv(PCINT0_vect)
#endif

template <class Pin, typename TimerDT, class mask, class flg>
class ButtonAssyncBase : public callback {
	protected:
		#ifdef PCINT0_vect
			friend ISRf(PCINT0_vect);
		#endif

		TimerDT time;
		bool reset_flag, state, prev_state;
		unsigned char clicks_count;

		void inline __attribute__((always_inline)) operator()(void) override {
			if (mask::isSet()) {
				if (state) {
					overflow_limit_sum(time, 1);
				}
			}
			else {
				_reset_timer();
			}
		}

		void inline __attribute__((always_inline)) interruptCallback(void) {
			state = real_state();
			memory();
			if (reset_flag) {
				if (!state) {
					_reset();
					reset_flag = false;
				}
			}
			else if (prev_state != state) {
				if (state) {
					_reset_timer();
				}
				else {
					overflow_limit_sum(clicks_count, 1);
				}
				memory();
				prev_state = state;
			}
		}

		void inline __attribute__((always_inline)) _reset_timer() {
			time = 0;
		}

		void inline __attribute__((always_inline)) _reset(void) {
			_reset_timer();
			clicks_count = 0;
		}

	public:
		ButtonAssyncBase(const bool PullUp = false) : time(0), clicks_count(0), reset_flag(false) {
			Pin::SetConfiguration(Pin::Port::In);
			if (PullUp) {
				Pin::SetPullUp(Pin::Port::PullUp);
			}
			else {
				Pin::SetPullUp(Pin::Port::NoPullUp);
			}
			memory();
			const bool tmp = real_state();
			prev_state = tmp;
			state = tmp;
		}

		inline unsigned char clicks(void) volatile {
			if (reset_flag) {
				return 0;
			}
			return clicks_count;
		}

		void inline __attribute__((always_inline)) reset(void) {
			reset_flag = true;
		}

		virtual TimerDT timer(void) volatile {
			if (reset_flag) {
				return 0;
			}
			return time;
		}

		void inline __attribute__((always_inline)) enable(void) {
			if (!mask::isSet()) {
				ATOMIC{
					mask::Set();
				}
			}
		}

		void inline __attribute__((always_inline)) disable(void) {
			if (mask::isSet()) {
				ATOMIC{
					mask::Clear();
				}
				memory();
				flg::Set();
			}
		}

		bool inline __attribute__((always_inline)) real_state(void) volatile {
			return Pin::IsSet();
		}

		inline __attribute__((always_inline)) operator bool(void) const volatile {
			return state;
		}
};

template <class Pin, typename TimerDT, class mask, class flg>
class ButtonAssync : public ButtonAssyncBase<Pin, TimerDT, mask, flg> {
	public:
		TimerDT timer(void) override {
			if (this->reset_flag) {
				return 0;
			}

			volatile const TimerDT tmp;
			memory();
			ATOMIC{
				tmp = this->time;
			}
			memory();
			return tmp;
		}
};

template <class Pin, class mask, class flg>
class ButtonAssync<Pin, unsigned char, mask, flg> : public ButtonAssyncBase<Pin, unsigned char, mask, flg> {};

#endif