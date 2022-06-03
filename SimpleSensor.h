#pragma once

#ifndef SIMPLESENSOR_H_
#define SIMPLESENSOR_H_

#include "Delays.h"
#include "Vector.h"
#include <avr/io.h>
#include "Common.h"

#ifdef INT0_vect
	ISRv(INT0_vect)
#endif

#ifdef INT1_vect
	ISRv(INT1_vect)
#endif

#ifdef INT2_vect
	ISRv(INT2_vect)
#endif

#ifdef INT3_vect
	ISRv(INT3_vect)
#endif

#ifdef PCINT0_vect
	ISRv(PCINT0_vect);
#endif

template <class Pin, typename DelayType, const DelayType DelayON, const DelayType DelayOFF>
class Sensor : public DelayedVal <bool, DelayType> {
	protected:
		#ifdef INT0_vect
			friend ISRf(INT0_vect);
		#endif

		#ifdef INT1_vect
			friend ISRf(INT1_vect);
		#endif

		#ifdef INT2_vect
			friend ISRf(INT2_vect);
		#endif

		#ifdef INT3_vect
			friend ISRf(INT3_vect);
		#endif

		#ifdef INT6_vect
			friend ISRf(INT6_vect);
		#endif

		#ifdef PCINT0_vect
			friend ISRf(PCINT0_vect);
		#endif

		bool inline __attribute__((always_inline)) real_val(void) {
			return Pin::IsSet();
		}

		DelayType get_timer_delay(bool state) override {
			return state ? DelayON : DelayOFF;
		}

	private:
		typedef DelayedVal <bool, DelayType> base;
		//using base::_value;

		void inline __attribute__((always_inline)) set_real_val(void) {
			this->value(real_val());
		}

		void inline __attribute__((always_inline)) _set_real_val(void) {
			this->_value(real_val());
		}

	public:
		//using base::atomic_wrapper_in;
		//using base::atomic_wrapper_out;

		void inline __attribute__((always_inline)) apply_real_val(void) {
			set_real_val();
		}

		void virtual inline __attribute__((always_inline)) interruptCallback(void) {
			_set_real_val();
		}

	public:
		Sensor(
			bool StartState,
			const bool PullUp) : base(StartState) {
				Pin::SetConfiguration(Pin::Port::In);
				if (PullUp) {
					Pin::SetPullUp(Pin::Port::PullUp);
				}
				else {
					Pin::SetPullUp(Pin::Port::NoPullUp);
				}
				memory();
				apply_real_val();
			};

		Sensor() : Sensor(real_val(), false) {}
};

//template <class Pin, typename DelayType, const DelayType DelayON, const DelayType DelayOFF, class timer_flag = Flag<IOReg<0>, 0>>
//class Sensor : public DelayedVal <bool, DelayType, timer_flag> {
//protected:
//#ifdef INT0_vect
//	friend ISRf(INT0_vect);
//#endif
//
//#ifdef INT1_vect
//	friend ISRf(INT1_vect);
//#endif
//
//#ifdef INT2_vect
//	friend ISRf(INT2_vect);
//#endif
//
//#ifdef INT3_vect
//	friend ISRf(INT3_vect);
//#endif
//
//#ifdef INT6_vect
//	friend ISRf(INT6_vect);
//#endif
//
//	inline const bool real_val(void) const {
//		return Pin::IsSet();
//	}
//
//	const DelayType get_timer_delay(const bool state) const override {
//		return state ? DelayON : DelayOFF;
//	}
//
//private:
//	typedef DelayedVal <bool, DelayType, timer_flag> base;
//
//	void set_real_val(void) {
//		this->value(this->real_val());
//	}
//
//protected:
//	//using base::atomic_wrapper_in;
//	//using base::atomic_wrapper_out;
//
//	void virtual apply_real_val(void) {
//		this->set_real_val();
//	}
//
//	inline virtual void interruptCallback(void) {
//		set_real_val();
//	}
//
//public:
//	Sensor(
//		const bool StartState,
//		const bool PullUp) : DelayedVal<bool, DelayType, timer_flag>(StartState) {
//		Pin::SetConfiguration(Pin::Port::In);
//		if (PullUp) {
//			Pin::SetPullUp(Pin::Port::PullUp);
//		}
//		else {
//			Pin::SetPullUp(Pin::Port::NoPullUp);
//		}
//		memory();
//		apply_real_val();
//	};
//
//	Sensor() : Sensor(real_val(), false) {}
//};
//
//template <class Pin, typename DelayType, const DelayType DelayON, const DelayType DelayOFF, class mask, class flag, class timer_flag = Flag<IOReg<0>, 0>>
//class SensorAtomic : public Sensor<Pin, DelayType, DelayON, DelayOFF, timer_flag> {
//private:
//	typedef Sensor<Pin, DelayType, DelayON, DelayOFF, timer_flag> sensorBase;
//
//	void inline atomic_val_wrapper_in(void) {
//		mask::Clear();
//		memory();
//		nop();
//		nop();
//		nop();
//		nop();
//		nop();
//	}
//
//	void inline atomic_val_wrapper_out(void) {
//		mask::Set();
//		memory();
//		nop();
//		memory();
//		//flag::Set();
//	}
//
//protected:
//#ifdef INT0_vect
//	friend ISRf(INT0_vect);
//#endif
//
//#ifdef INT1_vect
//	friend ISRf(INT1_vect);
//#endif
//
//#ifdef INT2_vect
//	friend ISRf(INT2_vect);
//#endif
//
//#ifdef INT3_vect
//	friend ISRf(INT3_vect);
//#endif
//
//#ifdef INT6_vect
//	friend ISRf(INT6_vect);
//#endif
//
//	/*void inline stage(const bool val) override {
//		sensorBase::atomic_wrapper_in();
//		sensorBase::stage(val);
//		sensorBase::atomic_wrapper_out();
//	}*/
//
//	void apply_real_val(void) override {
//		memory();
//		atomic_val_wrapper_in();
//		memory();
//		sensorBase::apply_real_val();
//		memory();
//		atomic_val_wrapper_out();
//		memory();
//	}
//
//public:
//	SensorAtomic(
//		const bool StartState,
//		const bool PullUp = false) : sensorBase(StartState, PullUp) {
//	};
//
//	SensorAtomic() : sensorBase() {}
//};

#endif