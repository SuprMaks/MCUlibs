#pragma once
#ifndef DELAYS_H_
#define DELAYS_H_

template <typename DataType, typename DelayType>
class DelayedVal: {
	protected:
		//friend ISRf(WDT_vect);
		//const DelayType _DelayON, _DelayOFF;
		DelayType _Timer;
		bool _TimerSync;
		DataType _Stage, _State;

		virtual const DelayType _getDelay(const DataType state) const = 0;

		virtual const DataType _realData(void) const = 0;

		void call(void) {
			if (this->_TimerSync) {
				if (this->_Stage != this->value()) {
					if (this->_Timer > 0) {
						this->_Timer--;
					}
					else {
						this->_State = this->_Stage;
						this->_Timer = _getDelay(this->_Stage);
					}
				}
			}
			else {
				this->_TimerSync = true;
			}
		}

	public:
		DelayedVal(const DataType StartState) : _TimerSync(false), _Stage(StartState), _State(StartState) {

		};
		DelayedVal(void) : _TimerSync(false) {

		};

		void value(const DataType state) {
			if (state != this->_Stage) {
				this->_Stage = state;
				this->_Timer = _getDelay(state);
				this->_TimerSync = false;
			}
		}

		const DataType value(void) const {
			return this->_State;
		}
};

#endif