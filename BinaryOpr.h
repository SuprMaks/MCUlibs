#pragma once

#ifndef BINARYOPR_H_
#define BINARYOPR_H_

#include <avr/sfr_defs.h>

#define bitmask_Set(val, mask) val |= mask
#define bitmask_Clear(val, mask) val &= ~mask
#define bitmask_Toogle(val, mask) val ^= mask
#define bitmask_Read_All(val, mask) (val & mask) == mask   // warning: evaluates y twice
#define bitmask_Read_Any(val, mask) val & mask

#define bit(b) _BV(b)
// (1UL << (b))
#define bitRead(value, b) bit_is_set(value, b)
// ((value >> b) & 0x01)
#define bitSet(value, b) bitmask_Set(value, bit(b))
#define bitClear(value, b) bitmask_Clear(value, bit(b))
#define bitToogle(value, b) bitmask_Toogle(value, bit(b))
#define bitWrite(value, b, bitvalue) (bitvalue ? bitSet(value, b) : bitClear(value, b))

#define lowByte(w) ((unsigned char) ((w) & 0xff))
#define highByte(w) ((unsigned char) ((w) >> 8))

#endif /* BINARYOPR_H_ */