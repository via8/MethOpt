#include "EigenHelper.hpp"
#include <limits.h>	// LLONG_MAX, INT_MAX
#include <limits>	// numeric_limits

#ifdef _WIN64
	Int const EigenHelper::InfinityInt = LLONG_MAX;
#else
	Int const EigenHelper::InfinityInt = INT_MAX;
#endif

float const EigenHelper::InfinityFloat = std::numeric_limits<float>::infinity();