#ifndef EIGEN_HELPER_H
#define EIGEN_HELPER_H

#include <Eigen/Dense>

using Int = Eigen::Index;
using VectorFloat = Eigen::VectorXf;
using MatrixFloat = Eigen::MatrixXf;
using PairsInt	  = Eigen::Matrix<Int, -1, 2>;

class EigenHelper {
private:
	 EigenHelper() = delete;
	~EigenHelper() = delete;

public:
	static float const InfinityFloat;
};

#endif /* EIGEN_HELPER_H */