#pragma once
#include "types.h"

namespace util {
	template<class A>
	class ManagedArray {
	private:
		A* array;
		count_t size;
	public:
		ManagedArray();
		ManagedArray(int);
	};
}
