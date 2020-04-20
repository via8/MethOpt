#ifndef GRADIENT_INPUT_HPP
#define GRADIENT_INPUT_HPP

#include <fstream>	// ifstream
#include "EigenHelper.hpp"

class GradientInput {
private:
	static char const* EXCEPTION_INVALID_INPUT_DATA;
	static char const* EXCEPTION_UNEXPECTED_EOF;

	int dim;
	VectorDouble initialPoint;
	double epsilon;

public:
	GradientInput(std::ifstream& input);
	~GradientInput();

	int getDim() const { return dim; }
	VectorDouble getInitialPoint() const { return initialPoint; }
	double getEpsilon() const { return epsilon; }
};

#endif /* GRADIENT_INPUT_HPP */