#pragma once

template<class T>
constexpr const T saturation(T val, T min, T max) {
	if (val > max) {
		return max;
	}
	else if (val < min) {
		return min;
	}
	return val;
}

template<class T, T min, T max>
constexpr const T saturation(T val) {
	if (val > max) {
		return max;
	}
	else if (val < min) {
		return min;
	}
	return val;
}