#ifndef GRADIENT_OUTPUT_HPP
#define GRADIENT_OUTPUT_HPP

#include <vector>
#include "EigenHelper.hpp"

class GradientOutput {
private:
	std::vector<VectorDouble> approxSequence;

public:
	GradientOutput() {};
	~GradientOutput() {};

	void putPoint(VectorDouble point) { approxSequence.push_back(point); }
	std::vector<VectorDouble> getApproxSequence() { return approxSequence; }
};

#endif /* GRADIENT_OUTPUT_HPP */