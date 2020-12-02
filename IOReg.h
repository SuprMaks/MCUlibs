#pragma once
#include "BinaryOpr.h"

namespace Private {
	template<unsigned char RegAddr, typename DATA_TYPE = unsigned char>
	struct Reg {
		static volatile DATA_TYPE& reg(void) {
			return *(DATA_TYPE*)RegAddr;
		}
	};
}

template<unsigned char RegAddr, typename DATA_TYPE = unsigned char>
struct IReg :public Private::Reg<RegAddr> {
	using Private::Reg<RegAddr>::reg;

	static DATA_TYPE read(void) {
		return reg();
	}

//protected:
	struct bit {
		template<unsigned char b>
		static inline bool read(void) {
			return bitRead(reg(), b);
		}
	};
};

template<unsigned char RegAddr, typename DATA_TYPE = unsigned char>
struct OReg : public Private::Reg<RegAddr> {
	using Private::Reg<RegAddr>::reg;

	void set(DATA_TYPE val) {
		reg() = val;
	}

//protected:
	struct bit {
		template<unsigned char b>
		static inline bool set(void) {
			return bitSet(reg(), b);
		}

		template<unsigned char b>
		static inline bool clear(void) {
			return bitClear(reg(), b);
		}

		template<unsigned char b>
		static inline bool toogle(void) {
			return bitToogle(reg(), b);
		}
	};
};

template<unsigned char RegAddr, typename DATA_TYPE = unsigned char>
struct IOReg : public IReg<RegAddr, DATA_TYPE>, public OReg<RegAddr, DATA_TYPE> {
	struct bit : public IReg<RegAddr, DATA_TYPE>::bit, public OReg<RegAddr, DATA_TYPE>::bit {

	};
};

//template<unsigned char RegAddr, typename DATA_TYPE = unsigned char>
//struct IOReg {
//	typedef DATA_TYPE DataT;
//
//	static volatile DataT& Value() { return *static_cast<DataT*>(RegAddr); }
//	static DataT Get() { return Value(); }
//	static void Set(DataT value) { Value() = value; }
//	static void Or(DataT value) { Value() |= value; }
//	static void And(DataT value) { Value() &= value; }
//	static void Xor(DataT value) { Value() ^= value; }
//	static void AndOr(DataT andMask, DataT orMask) { Value() = (Value() & andMask) | orMask; }
//	template<int Bit>
//	static bool BitIsSet() { return Value() & (1 << Bit); }
//	template<int Bit>
//	static bool BitIsClear() { return !(Value() & (1 << Bit)); }
//};