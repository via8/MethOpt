#ifndef EIGEN_HELPER_HPP
#define EIGEN_HELPER_HPP

#include <Eigen/Dense>

using Int		  = Eigen::Index;
using VectorInt   = Eigen::VectorXi;
using VectorFloat = Eigen::VectorXf;
using MatrixInt	  = Eigen::MatrixXi;
using MatrixFloat = Eigen::MatrixXf;

class EigenHelper {
private:
	 EigenHelper() = delete;
	~EigenHelper() = delete;

public:
	static Int const InfinityInt;
	static float const InfinityFloat;
	static float const NanFloat;
	static bool isNan(float value);
	static Int matrixFloatRank(MatrixFloat const& matrix);
};

#endif /* EIGEN_HELPER_HPP */