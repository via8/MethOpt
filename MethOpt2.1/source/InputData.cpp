#include "InputData.h"

char const* InputData::EXCEPTION_INVALID_INPUT_DATA = "Unable to read input data, check validity";
char const* InputData::EXCEPTION_INVALID_INTERVAL	= "Invalid interval borders, make sure a < b";
char const* InputData::EXCEPTION_INVALID_PRECISION	= "Invalid precision, make sure epsilon > 0";


InputData::InputData(std::ifstream& input) {
	// read interval borders
	if (!(input >> m_a))
		throw EXCEPTION_INVALID_INPUT_DATA;
	if (!(input >> m_b))
		throw EXCEPTION_INVALID_INPUT_DATA;
	if (m_a >= m_b)
		throw EXCEPTION_INVALID_INTERVAL;

	// read precision
	if (!(input >> m_epsilon) || m_epsilon <= 0)
		throw EXCEPTION_INVALID_INPUT_DATA;
}