#pragma once

#ifndef VECTOR_H_
#define VECTOR_H_

#ifdef __cplusplus
#define ISRv(vector, ...) extern "C" void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__;
#else
#define ISRv(vector, ...) void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__;
#endif

#define ISRf(vector, ...) void vector (void)

#endif