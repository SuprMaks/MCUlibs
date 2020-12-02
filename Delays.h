#pragma once

#ifndef DELAYS_H_
#define DELAYS_H_

#include "SingleTone.h"
#include "Vector.h"
#include "Callback.h"
#include <avr/io.h>
#include "atomic.h"
#include "Common.h"
#include "BinaryOpr.h"
//#include <stdlib.h>

#ifdef WDT_vect
	ISRv(WDT_vect)
#endif

class WatchdogCallbackLoop : public CallbackLoop {
	friend class Singleton<WatchdogCallbackLoop>;
};

template <typename TimerDT>
class TimerBase : public callback {
	protected:
		TimerDT time;
		bool time_sync;

		virtual void handler_timer_done(void) {
			return;
		}

		void operator()(void) override {
			if (this->time_sync) {
				if (this->time > 0) {
					this->time--;
				}
				else {
					handler_timer_done();
				}
			}
			else {
				this->time_sync = true;
			}
		}

	public:
		TimerBase(void) : time_sync(false), time(0) {}

		void timer(const TimerDT delay) {
			this->time = delay;
			this->time_sync = false;
		}

		virtual const TimerDT timer(void) {
			return this->time;
		}

		const bool isDone(void) {
			return timer() == 0;
		}
};

template <typename TimerDT, class flag = Flag<IOReg<0x3F + __SFR_OFFSET>, SREG_I>>
class TimerBaseAtomic : public TimerBase<TimerDT> {
	private:
		typedef TimerBase<TimerDT> base;

	protected:
		bool interrupt_enabled, interrupt_flag;

		void operator()(void) override {
			if (interrupt_enabled) {
				base::operator()();
			}
			else {
				interrupt_flag = true;
			}
		}

		class Atomic {
			public:
				inline Atomic(void) {
					while (flag::isSet());
					interrupt_enabled = false;
				}

				inline ~Atomic(void) {
					interrupt_enabled = true;
					if (interrupt_flag) {
						operator()();
						interrupt_flag = false;
					}
				}

				inline operator bool() const {
					return false;
				}
		};

	public:
		TimerBaseAtomic(void) : base(), interrupt_enabled(true), interrupt_flag(false) {}

		void timer(const TimerDT delay) {
			if (Atomic di = Atomic()) {}
			else {
				base::timer(delay);
			}
		}

		const TimerDT timer(void) override {
			volatile TimerDT tmp;
			if (Atomic di = Atomic()) {}
			else {
				tmp = base::timer();
			}
			return tmp;
		}
};

template <typename DataType, typename DelayType, class timer_flag = Flag<IOReg<0>, 0>>
class DelayedVal : public TimerBaseAtomic<DelayType, timer_flag> {
	private:
		typedef TimerBaseAtomic<DelayType, timer_flag> base;

	protected:
		
		#ifdef WDT_vect
			friend ISRf(WDT_vect);
		#endif
		DataType stage_val, curr_val;

		virtual const DelayType get_timer_delay(const DataType state) const = 0;

		void inline handler_timer_done(void) override {
			this->curr_val = this->stage_val;
		}

		void value(const DataType val) {
			if (val != this->stage_val) {
				this->stage_val = val;
				this->timer(this->get_timer_delay(val));
			}
		}

	public:
		DelayedVal(const DataType StartState) : base(), stage_val(StartState), curr_val(StartState) {};
		DelayedVal(void) : base() {};

		const inline DataType value(void) const {
			return this->curr_val;
		}
};

template <typename TimerDT, class flag = Flag<IOReg<0>, 0>>
class Timer : public TimerBaseAtomic<TimerDT, flag> {
	private:
		typedef TimerBaseAtomic<TimerDT, flag> base;
		#ifdef WDT_vect
			friend ISRf(WDT_vect);
		#endif
		unsigned char timer_id;

	public:
		const bool operator()(const unsigned char Tid, const TimerDT delay) {
			if (this->timer_id == Tid) {
				if (this->isDone()) {
					this->timer(delay);
					return true;
				}
			}
			else {
				this->timer(Tid, delay);
			}
			return false;
		}

		Timer(unsigned char id = 0) : base(), timer_id(id) {}

		using base::timer;

		void timer(const unsigned char Tid, const TimerDT delay) {
			this->timer_id = Tid;
			this->timer(delay);
		}
};

#endif