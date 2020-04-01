#include "EigenHelper.hpp"
#include <limits.h>	// LLONG_MAX, INT_MAX
#include <limits>	// numeric_limits
#include <cmath>	// nan

#ifdef _WIN64
	Int const EigenHelper::InfinityInt = LLONG_MAX;
#else
	Int const EigenHelper::InfinityInt = INT_MAX;
#endif

#define THRESHOLD 1e-3f

float const EigenHelper::InfinityFloat = std::numeric_limits<float>::infinity();
float const EigenHelper::NanFloat = std::nanf(0);

bool EigenHelper::isNan(float value) {
	return std::isnan(value);
}

Int EigenHelper::matrixFloatRank(MatrixFloat const& matrix) {
	// by this Eigen library manipulation with LU-decomposition of our matrix
	// we can calculate it's rank with specified threshold
	Eigen::FullPivLU<MatrixFloat> lu(matrix);
	lu.setThreshold(THRESHOLD);
	return lu.rank();
}