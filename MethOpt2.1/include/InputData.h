#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <fstream> // ifstream

class InputData {
private:
	static char const* EXCEPTION_INVALID_INPUT_DATA;
	static char const* EXCEPTION_INVALID_INTERVAL;
	static char const* EXCEPTION_INVALID_PRECISION;

	double m_a;
	double m_b;
	double m_epsilon;

public:
	InputData(std::ifstream& input);
	InputData(double a, double b, double epsilon);
	double geta() const { return m_a; }
	double getb() const { return m_b; }
	double getEpsilon() const { return m_epsilon; }
};

#endif INPUT_DATA_H