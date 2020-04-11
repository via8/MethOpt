#ifndef FUNCTION_H
#define FUNCTION_H

#include <functional>

class Function {
private:
	std::function<double(double)> m_function;
	int m_callCount;

public:
	Function(std::function<double(double)> function);
	~Function();

	// function call
	double operator()(double x);
	int getCallCount() { return m_callCount; }
	void resetCallCount() { m_callCount = 0; }
};

#endif /* FUNCTION_H */