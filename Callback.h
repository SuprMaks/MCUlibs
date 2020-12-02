#pragma once

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include "SimpleVector.h"

class callback {
	public:
		virtual void operator()(void) = 0;
};

#if __cplusplus >= 201500
template <auto Func>
struct Callback {
	void static exec(void) {
		Func();
	}
};
#endif

typedef SimpleVector<callback> CallbackLoop;

typedef void (*func_void) (void);

#endif