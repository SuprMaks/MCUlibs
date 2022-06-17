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

//template <typename TimerDT>
//class TimerBase : public callback {
//protected:
//	TimerDT time;
//	bool time_sync;
//
//	virtual void handler_timer_done(void) {
//		return;
//	}
//
//	void inline operator()(void) override {
//		if (time_sync) {
//			if (time > 0) {
//				time--;
//			}
//			else {
//				handler_timer_done();
//			}
//		}
//		else {
//			time_sync = true;
//		}
//	}
//
//	virtual void _timer(const TimerDT delay) {
//		this->time = delay;
//		memory();
//		this->time_sync = false;
//	}
//
//	virtual const TimerDT _timer(void) {
//		volatile TimerDT tmp = time;
//		memory();
//		return tmp;
//	}
//
//	inline const bool _isDone(void) {
//		return _timer() == 0;
//	}
//
//public:
//	TimerBase(void) : time_sync(false), time(0) {}
//
//	virtual void atomic_timer(const TimerDT delay) {
//		ATOMIC{
//			timer(delay);
//		}
//	}
//
//	virtual const TimerDT atomic_timer(void) {
//		volatile TimerDT tmp;
//		memory();
//		ATOMIC{
//			tmp = timer();
//		}
//		return tmp;
//	}
//
//	inline const bool atomic_isDone(void) {
//		return timer() == 0;
//	}
//};

template <typename TimerDT>
class TimerProt : public callback {
	protected:
		TimerDT time;

		void virtual inline __attribute__((always_inline)) handler_timer_done(void) {
			return;
		}

		void inline __attribute__((always_inline)) operator()(void) override {
			if (time > 0) {
				time--;
			}
			else {
				handler_timer_done();
			}
		}

		virtual void inline _timer(TimerDT delay) {
			//time = (delay == 0) ? 0 : delay + 1;
			//time = delay + 1;
			time = delay;
		}

		virtual TimerDT inline _timer(void) volatile {
			return time;
		}

		bool inline _isDone(void) volatile {
			return time == 0;
		}

	public:
		TimerProt(void) : time(0) {}
};

template <typename TimerDT>
class TimerBase : public TimerProt<TimerDT> {
	private:
		typedef TimerProt<TimerDT> base;

	/*protected:
		using base::_timer;*/

	public:
		TimerBase(void) {
			ATOMIC{
				base();
			}
		}

		virtual void timer(TimerDT delay) {
			ATOMIC{
				this->_timer(delay);
			}
		}

		virtual TimerDT timer(void) {
			volatile TimerDT tmp;
			memory();
			ATOMIC{
				tmp = this->_timer();
			}
			memory();
			return tmp;
		}

		bool inline __attribute__((always_inline)) isDone(void) {
			return timer() == 0;
		}
};

template <>
class TimerBase<unsigned char> : public TimerProt<unsigned char> {
	private:
		typedef TimerProt<unsigned char> base;

	public:
		virtual void inline __attribute__((always_inline)) timer(unsigned char delay) {
			_timer(delay);
		}

		virtual unsigned char inline __attribute__((always_inline)) timer(void) {
			return _timer();
		}

		bool inline __attribute__((always_inline)) isDone(void) {
			return timer() == 0;
		}
};

//template <typename TimerDT, class flag>
//class TimerProtAtomic : public TimerBase<TimerDT> {
//	protected:
//		typedef TimerBase<TimerDT> base;
//		bool interrupt_enabled, interrupt_flag;
//
//		void inline operator()(void) override {
//			if (interrupt_enabled) {
//				base::operator()();
//			}
//			else {
//				interrupt_flag = true;
//			}
//		}
//
//		inline void atomic_wrapper_in(void) {
//			interrupt_enabled = false;
//
//			memory();
//			while (flag::isSet());
//			memory();
//		}
//
//		inline void atomic_wrapper_out(void) {
//			if (interrupt_flag) {
//				base::operator()();
//				memory();
//				interrupt_flag = false;
//			}
//			memory();
//			interrupt_enabled = true;
//			memory();
//		}
//
//	public:
//		TimerProtAtomic(void) : base(), interrupt_enabled(true), interrupt_flag(false) {}
//
//		void timer(const TimerDT delay) override {
//			memory();
//			atomic_wrapper_in();
//			memory();
//			base::timer(delay);
//			memory();
//			atomic_wrapper_out();
//			memory();
//		}
//
//		const TimerDT timer(void) override {
//			memory();
//			atomic_wrapper_in();
//			memory();
//			volatile TimerDT tmp = base::timer();
//			memory();
//			atomic_wrapper_out();
//			memory();
//			return tmp;
//		}
//};

//template <typename TimerDT>
//class TimerProtAtomic<TimerDT, Flag<IOReg<0>, 0>> : public TimerBase<TimerDT> {
//	protected:
//		typedef TimerBase<TimerDT> base;
//
//		inline void atomic_wrapper_in(void) {}
//		inline void atomic_wrapper_out(void) {}
//
//	public:
//		TimerProtAtomic(void) : base() {}
//};
//
//template <typename TimerDT, class flag = Flag<IOReg<0>, 0>>
//class TimerAtomic : public TimerProtAtomic<TimerDT, flag> {
//	private:
//		typedef TimerProtAtomic<TimerDT, flag> baseTimer;
//		typedef baseTimer::base base;
//
//	protected:
//		using baseTimer::atomic_wrapper_in;
//		using baseTimer::atomic_wrapper_out;
//};
//
//template <class flag>
//class TimerAtomic<unsigned char, flag> : public TimerProtAtomic<unsigned char, flag> {
//	private:
//		typedef TimerProtAtomic<unsigned char, flag> baseTimer;
//		typedef baseTimer::base base;
//	
//	protected:
//		using baseTimer::atomic_wrapper_in;
//		using baseTimer::atomic_wrapper_out;
//
//	public:
//		using baseTimer::timer;
//
//		const unsigned char timer(void) override {
//			return base::timer();
//		}
//};

template <typename DataType, typename DelayType>
class DelayedVal : public TimerBase<DelayType> {
	private:
		typedef TimerBase<DelayType> base;

	protected:
		#ifdef WDT_vect
			friend ISRf(WDT_vect);
		#endif
		DataType stage_val, curr_val;

		virtual DelayType get_timer_delay(DataType state) = 0;

		void inline __attribute__((always_inline)) handler_timer_done(void) override {
			curr_val = stage_val;
		}

		void _value(DataType val) {
			if (val != stage_val) {
				this->_timer(get_timer_delay(val));
				memory();
				//stage(val);
				stage_val = val;
			}
		}

	public:
		DelayedVal(DataType StartState) : base(), stage_val(StartState), curr_val(StartState) {};
		DelayedVal(void) : base() {};

		void value(DataType val) {
			if (val != stage_val) {
				this->timer(get_timer_delay(val));
				memory();
				//stage(val);
				stage_val = val;
			}
		}

		// TODO: Thhis is not safe/atomic funct
		DataType inline __attribute__((always_inline)) value(void) volatile {
			return curr_val;
		}

		// TODO: Thhis is not safe/atomic funct
		bool inline __attribute__((always_inline)) val_in_sync(void) volatile {
			return curr_val == stage_val;
		}
};

template <typename TimerDT>
class Timer : public TimerBase<TimerDT> {
	private:
		typedef TimerBase<TimerDT> base;
		#ifdef WDT_vect
			friend ISRf(WDT_vect);
		#endif
		unsigned char timer_id;

	protected:
		using base::_timer;

		void _timer(unsigned char Tid, TimerDT delay) {
			id(Tid);
			memory();
			_timer(delay);
		}

	public:
		bool operator()(unsigned char Tid, TimerDT delay) {
			if (timer_id == Tid) {
				if (this->isDone()) {
					timer(delay);
					return true;
				}
			}
			else {
				timer(Tid, delay);
			}
			return false;
		}

		Timer(unsigned char id = 0): timer_id(id) {}

		using base::timer;

		void timer(unsigned char Tid, TimerDT delay) {
			id(Tid);
			memory();
			timer(delay);
		}

		unsigned char inline __attribute__((always_inline)) id(void) volatile {
			return timer_id;
		}

		void inline id(unsigned char Tid) {
			timer_id = Tid;
		}
};

//template <typename DataType, typename DelayType, class timer_flag = Flag<IOReg<0>, 0>>
//class DelayedVal : public TimerAtomic<DelayType, timer_flag> {
//private:
//	typedef TimerAtomic<DelayType, timer_flag> base;
//
//protected:
//
//#ifdef WDT_vect
//	friend ISRf(WDT_vect);
//#endif
//	DataType stage_val, curr_val;
//
//	//using base::atomic_wrapper_in;
//	//using base::atomic_wrapper_out;
//
//	virtual const DelayType get_timer_delay(const DataType state) const = 0;
//
//	void inline handler_timer_done(void) override {
//		this->curr_val = this->stage_val;
//	}
//
//	/*void virtual inline stage(const DataType val) {
//		this->stage_val = val;
//	}*/
//
//	void value(const DataType val) {
//		if (val != this->stage_val) {
//			this->timer(this->get_timer_delay(val));
//			memory();
//			//stage(val);
//			stage_val = val;
//		}
//	}
//
//public:
//	DelayedVal(const DataType StartState) : base(), stage_val(StartState), curr_val(StartState) {};
//	DelayedVal(void) : base() {};
//
//	const inline DataType value(void) const {
//		return this->curr_val;
//	}
//};
//
//template <typename TimerDT, class flag = Flag<IOReg<0>, 0>>
//class Timer : public TimerAtomic<TimerDT, flag> {
//private:
//	typedef TimerAtomic<TimerDT, flag> base;
//#ifdef WDT_vect
//	friend ISRf(WDT_vect);
//#endif
//	unsigned char timer_id;
//
//public:
//	const bool operator()(const unsigned char Tid, const TimerDT delay) {
//		if (this->timer_id == Tid) {
//			if (this->isDone()) {
//				this->timer(delay);
//				return true;
//			}
//		}
//		else {
//			this->timer(Tid, delay);
//		}
//		return false;
//	}
//
//	Timer(unsigned char id = 0) : base(), timer_id(id) {}
//
//	using base::timer;
//
//	void timer(const unsigned char Tid, const TimerDT delay) {
//		this->timer_id = Tid;
//		this->timer(delay);
//	}
//
//	const unsigned char inline id(void) {
//		return this->timer_id;
//	}
//
//	void inline id(const unsigned char Tid) {
//		this->timer_id = Tid;
//	}
//};

#endif