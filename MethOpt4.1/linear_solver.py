import linear_problem
import subprocess


def get_order(value):
    order = 1
    temp = 0.1
    while temp > value:
        order += 1
        temp /= 10
    return order


def run(lin_prob: linear_problem.LinearProblem, epsilon):
    _input = open("input.txt", "w+")
    _input.write(str(get_order(epsilon)) + '\n')
    _input.write(str(lin_prob.n) + ' ' + str(lin_prob.m) + '\n')
    for i in range(0, lin_prob.m):
        for j in range(0, lin_prob.n):
            _input.write(str(lin_prob.A[i][j]) + ' ')
        _input.write(lin_prob.signs[i] + str(lin_prob.b[i]) + '\n')
    for j in range(0, lin_prob.n):
        _input.write(str(lin_prob.c[j]) + ' ')
    _input.close()
    subprocess.call("..\\MethOpt1.1\\x64\\Release\\MethOpt1.exe input.txt output.txt", shell=True)

    _output = open("output.txt", "r")
    result = [float(item) for item in _output.read().split()]
    _output.close()
    return result
