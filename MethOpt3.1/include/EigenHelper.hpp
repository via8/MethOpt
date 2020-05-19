#ifndef EIGEN_HELPER_HPP
#define EIGEN_HELPER_HPP

#include "eigen-3.3.7/Eigen/Dense"

using Int			= Eigen::Index;
using VectorDouble	= Eigen::VectorXd;
using MatrixDouble	= Eigen::MatrixXd;

class EigenHelper {
private:
	EigenHelper() = delete;
	~EigenHelper() = delete;

public:
	static VectorDouble solveLinearDense(MatrixDouble A, VectorDouble b);
};

#endif /* EIGEN_HELPER_HPP */
