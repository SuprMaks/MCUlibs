#pragma once

#ifndef SIMPLESENSOR_H_
#define SIMPLESENSOR_H_

#include "Delays.h"
#include "Vector.h"
#include <avr/io.h>

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

		const bool real_val(void) const {
			return Pin::IsSet();
		}

		void apply_real_val(void) {
			this->value(this->real_val());
		}

		const DelayType get_timer_delay(const bool state) const override {
			return state ? DelayON : DelayOFF;
		} 

		virtual void interruptCallback(void) {
			apply_real_val();
		}

	public:
		Sensor(
			const bool StartState,
			const bool PullUp = false) : DelayedVal<bool, DelayType>(StartState) {
				Pin::SetConfiguration(Pin::Port::In);
				if (PullUp) {
					Pin::SetPullUp(Pin::Port::PullUp);
				}
				else {
					Pin::SetPullUp(Pin::Port::NoPullUp);
				}
				apply_real_val();
			};
};

#endif