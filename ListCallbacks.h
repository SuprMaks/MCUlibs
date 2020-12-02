#pragma once

#include "loki/NullType.h"
#include "loki/Typelist.h"
#include "Callback.h"

namespace List {
	using namespace Loki;
	
	template <class TList> struct CallbacksIterator;
	
	template <> struct CallbacksIterator<NullType> {
		static void exec(void) {};
	};
	
	template <class Head, class Tail>
		struct CallbacksIterator < Typelist<Head, Tail> > {
				static void exec(void) {
					Head::exec();
					CallbacksIterator<Tail>::exec();
				}
		};

	template<class Callbacks>
	struct CallbackSet {
		static void exec() {
			CallbacksIterator <Callbacks>::exec();
		}
	};

	template
		<
			typename T1 = NullType, typename T2 = NullType, typename T3 = NullType,
			typename T4 = NullType, typename T5 = NullType, typename T6 = NullType,
			typename T7 = NullType, typename T8 = NullType, typename T9 = NullType,
			typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
			typename T13 = NullType, typename T14 = NullType, typename T15 = NullType,
			typename T16 = NullType, typename T17 = NullType, typename T18 = NullType
		>
			struct CallbackList: public CallbackSet
				<
						typename TL::MakeTypelist
						<
							T1,
							T2, T3, T4,
							T5, T6, T7,
							T8, T9, T10,
							T11, T12, T13,
							T14, T15, T16,
							T17, T18
						>::Result
				> {};
}