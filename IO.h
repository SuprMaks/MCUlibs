#ifndef IO_H_
#define IO_H_

#include "Pin.h"

class Output : private Pin {
	public:
		Output(volatile unsigned char *ddr, volatile unsigned char *port, volatile unsigned char *pins, const unsigned char pin, const bool defaultState = false);
	
		const bool State (void) const;
	
		void On (void);
	
		void Off (void);
	
		void SetState (const bool State);
};

class Input : private Pin {
	public:
		Input(volatile unsigned char *ddr, volatile unsigned char *port, volatile unsigned char *pins, const unsigned char pin, const bool PullUp = false);
		
		void pullUp (const bool PullUp);
		
		const bool state (void) const;
};

#endif