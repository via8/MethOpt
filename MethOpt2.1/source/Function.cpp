#include "Function.h"

Function::Function(std::function<double(double)> function) :
	m_function(function), m_callCount(0) {}

Function::~Function() {}

double Function::operator()(double x) {
	++m_callCount;
	return m_function(x);
}

