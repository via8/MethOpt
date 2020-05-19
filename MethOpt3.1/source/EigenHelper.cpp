#include "EigenHelper.hpp"

VectorDouble EigenHelper::solveLinearDense(MatrixDouble A, VectorDouble b) {
	Eigen::JacobiSVD<MatrixDouble> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
	return svd.solve(b);
}