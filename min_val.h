#pragma once

#include <limits.h>

template<typename T>
constexpr T min(T a);

template<>
constexpr signed char min(signed char a) {
	return SCHAR_MIN;
}

template<>
constexpr unsigned char min(unsigned char a) {
	return 0x0u;
}

template<>
constexpr signed short int min(signed short int a) {
	return SHRT_MIN;
}

template<>
constexpr unsigned short int min(unsigned short int a) {
	return 0x0u;
}

template<>
constexpr signed int min(signed int a) {
	return INT_MIN;
}

template<>
constexpr unsigned int min(unsigned int a) {
	return 0x0u;
}

template<>
constexpr signed long int min(signed long int a) {
	return LONG_MIN;
}

template<>
constexpr unsigned long int min(unsigned long int a) {
	return 0x0ul;
}