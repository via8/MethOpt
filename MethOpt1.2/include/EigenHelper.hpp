#ifndef EIGEN_HELPER_HPP
#define EIGEN_HELPER_HPP

#include <Eigen/Dense>

using Int		  = Eigen::Index;
using VectorInt   = Eigen::VectorXi;
//using VectorFloat = Eigen::VectorXf;
using MatrixInt	  = Eigen::MatrixXi;
using MatrixFloat = Eigen::MatrixXf;
//using PairsInt	  = Eigen::Matrix<Int, -1, 2>;

//enum {
//	NO_CHANGE = Eigen::NoChange
//};

class EigenHelper {
private:
	 EigenHelper() = delete;
	~EigenHelper() = delete;

public:
	static Int const InfinityInt;
	static float const InfinityFloat;
};

#endif /* EIGEN_HELPER_HPP */