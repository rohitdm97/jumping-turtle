#pragma once
#include "types.h"

#include <map>

namespace util {
	template<typename A>
	using SparsedArray = std::map<index_t, A>;
}
