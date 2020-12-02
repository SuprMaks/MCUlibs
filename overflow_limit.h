#pragma once

#include "min_val.h"
#include "max_val.h"

template<class a, typename b>
constexpr const a& overflow_limit_sum(a& va, b vb) {
	if (vb > 0) {
		if (va > max(va) - vb) {
			va = max(va);
		}
		else {
			va += vb;
		}
	}
	else {
		if (va < min(va) - vb) {
			va = min(va);
		}
		else {
			va += vb;
		}
	}
	return va;
}

template<class a, typename b>
constexpr const a& overflow_limit_sum(a& va, b vb, a smin, a smax) {
	if (vb > 0) {
		if (va > smax - vb) {
			va = smax;
		}
		else {
			va += vb;
		}
	}
	else {
		if (va < smin - vb) {
			va = smin;
		}
		else {
			va += vb;
		}
	}
	return va;
}