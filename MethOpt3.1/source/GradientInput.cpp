#include "GradientInput.hpp"

char const* GradientInput::EXCEPTION_INVALID_INPUT_DATA = "Unable to read input data, check validity";
char const* GradientInput::EXCEPTION_UNEXPECTED_EOF		= "Unexpected end of file reached";

GradientInput::GradientInput(std::ifstream& input) {
	if (!(input >> dim) || dim <= 0)
		throw EXCEPTION_INVALID_INPUT_DATA;

	initialPoint = VectorDouble::Zero(dim);
	for (Int i = 0; i < dim; ++i) {
		if (input.eof())
			throw EXCEPTION_UNEXPECTED_EOF;
		if (!(input >> initialPoint[i]))
			throw EXCEPTION_INVALID_INPUT_DATA;
	}

	if (!(input >> epsilon))
		throw EXCEPTION_INVALID_INPUT_DATA;
}

GradientInput::~GradientInput() {}