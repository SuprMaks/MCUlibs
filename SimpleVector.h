#pragma once

#ifndef SIMPLEVECTOR_H_
#define SIMPLEVECTOR_H_

#include <stdlib.h>

void * operator new(size_t size);

void operator delete(void * ptr);

template <typename DataType>
class SimpleVector {
	private:
		struct VectorElem {
			DataType* elem;
			VectorElem* next;
		};
		VectorElem* first;
		VectorElem* current;
		bool first_filled;
	
	
	public:
		SimpleVector (void): first(new VectorElem), current(first), first_filled(false) {};
		SimpleVector (DataType* sens) {
			this->first= new VectorElem;
			this->first_filled = false;
			this->current= first;
			this->add(sens);	
		};
	
		SimpleVector* add (DataType* sens) {
			VectorElem* elem = first_filled ? new VectorElem : this->first;
			this->current->next = elem;
			elem->elem = sens;
			elem->next = this->first;
			this->current = elem;
			this->first_filled = true;
			return this;
		};
		
		DataType* next (void) {
			this->current = this->current->next;
			return this->current->elem;
		};
};

#endif