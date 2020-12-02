#pragma once

#include <limits.h>

template<typename T>
constexpr T max(T a);

template<>
constexpr signed char max(signed char a) {
	return SCHAR_MAX;
}

template<>
constexpr unsigned char max(unsigned char a) {
	return UCHAR_MAX;
}

template<>
constexpr signed short int max(signed short int a) {
	return SHRT_MAX;
}

template<>
constexpr unsigned short int max(unsigned short int a) {
	return USHRT_MAX;
}

template<>
constexpr signed int max(signed int a) {
	return INT_MAX;
}

template<>
constexpr unsigned int max(unsigned int a) {
	return UINT_MAX;
}

template<>
constexpr signed long int max(signed long int a) {
	return LONG_MAX;
}

template<>
constexpr unsigned long int max(unsigned long int a) {
	return ULONG_MAX;
}