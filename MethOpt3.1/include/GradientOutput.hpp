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
	std::vector<VectorDouble> getApproxSequence() const { return approxSequence; }
	void clearApproxSequence() { approxSequence.clear(); approxSequence.resize(0); }
};

#endif /* GRADIENT_OUTPUT_HPP */