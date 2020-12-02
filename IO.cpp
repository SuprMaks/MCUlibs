#include "IO.h"

Output::Output(volatile unsigned char *ddr, volatile unsigned char *port, volatile unsigned char *pins, const unsigned char pin, const bool defaultState): Pin(ddr, port, pins, pin) {
	Pin::setPORT(defaultState);
	Pin::setDDR(true);
}
	
const bool Output::State (void) const {
	return Pin::readPORT();
	//return bitRead(PORTB, this->Pin);
}
	
void Output::On (void) {
	Pin::setPORT(true);
}
	
void Output::Off (void) {
	Pin::setPORT(false);
}
	
void Output::SetState (const bool State) {
	Pin::setPORT(State);
}



Input::Input(volatile unsigned char *ddr, volatile unsigned char *port, volatile unsigned char *pins, const unsigned char pin, const bool PullUp ): Pin(ddr, port, pins, pin) {
	this->pullUp(PullUp);
	Pin::setDDR(false);
}

void Input::pullUp (const bool PullUp) {
	Pin::setPORT(PullUp);
}

const bool Input::state (void) const {
	return Pin::readPINS();
}