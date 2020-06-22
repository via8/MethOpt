from numpy import sqrt, cos, sin, arctan, abs, pi, zeros, linspace
from scipy.optimize import minimize
from matplotlib import pyplot
import os

WORK_DIR = os.path.dirname(os.path.abspath(__file__))
OUT_DIR = WORK_DIR + '\\doc\\res\\out\\'
CORRIDOR_WIDTH = 1.0
A_PARAM = 0.0
A_ARRAY = [CORRIDOR_WIDTH * 0.333, CORRIDOR_WIDTH * 0.400,
           CORRIDOR_WIDTH * 0.437, CORRIDOR_WIDTH * 0.4381,
           CORRIDOR_WIDTH * 0.460, CORRIDOR_WIDTH * 0.499]
DELTA = 1e-5
EPS = 1e-6
ROOT_5 = sqrt(5.0)
ADDITIONAL_ANGLE = arctan(0.5)


def objective(x):
    return x[0] - x[1] - x[2]


def x1(x):
    return x[0] + A_PARAM * sin(x[2])


def y1(x):
    return x[1] + A_PARAM * cos(x[2])


def x2(x):
    return x[0] + A_PARAM * ROOT_5 * cos(x[2] - ADDITIONAL_ANGLE)


def y2(x):
    return x[1] - A_PARAM * ROOT_5 * sin(x[2] - ADDITIONAL_ANGLE)


def x3(x):
    return x[0] + A_PARAM * ROOT_5 * cos(x[2] + ADDITIONAL_ANGLE)


def y3(x):
    return x[1] - A_PARAM * ROOT_5 * sin(x[2] + ADDITIONAL_ANGLE)


def x4(x):
    return x[0] - A_PARAM * sin(x[2])


def y4(x):
    return x[1] - A_PARAM * cos(x[2])


def ineq1(x):
    return x[0] - A_PARAM


def ineq2(x):
    return y3(x)


def ineq3(x):
    X1 = x1(x)
    Y1 = y1(x)
    X2 = x2(x)
    Y2 = y2(x)
    A = Y1 - Y2
    B = X2 - X1
    C = X1 * Y2 - X2 * Y1
    return (A * CORRIDOR_WIDTH + B * CORRIDOR_WIDTH + C) / sqrt(A * A + B * B) - DELTA


steps = []


def callback_trace(x):
    steps.append(x)


constraint_1 = {'type': 'ineq', 'fun': ineq1}
constraint_2 = {'type': 'ineq', 'fun': ineq2}
constraint_3 = {'type': 'ineq', 'fun': ineq3}
constraints = ([constraint_1, constraint_2, constraint_3])

if __name__ == '__main__':
    for test_num in range(0, len(A_ARRAY)):
        A_PARAM = A_ARRAY[test_num]
        bounds = [
            (A_PARAM, 3.0 * CORRIDOR_WIDTH),
            (A_PARAM, 3.0 * CORRIDOR_WIDTH),
            (0.0, pi / 2)
        ]

        n = 3
        x0 = zeros(n)
        x0[0] = 1.0 * CORRIDOR_WIDTH
        x0[1] = 0.5 * CORRIDOR_WIDTH
        x0[2] = 0.0

        assert(bounds[0][0] <= x0[0] <= bounds[0][1])
        assert(bounds[1][0] <= x0[1] <= bounds[1][1])
        assert(bounds[2][0] <= x0[2] <= bounds[2][1])
        assert(2.0 * A_PARAM < CORRIDOR_WIDTH - DELTA)

        steps.clear()
        steps.append(x0)
        minimize(objective, x0,
                 method='SLSQP',
                 bounds=bounds,
                 constraints=constraints,
                 options={'eps': 1e-6},
                 callback=callback_trace)

        pyplot.figure()
        pyplot.plot([0.0 * CORRIDOR_WIDTH, 3.0 * CORRIDOR_WIDTH], [0.0 * CORRIDOR_WIDTH, 0.0 * CORRIDOR_WIDTH], 'r')
        pyplot.plot([0.0 * CORRIDOR_WIDTH, 0.0 * CORRIDOR_WIDTH], [0.0 * CORRIDOR_WIDTH, 3.0 * CORRIDOR_WIDTH], 'r')
        pyplot.plot([1.0 * CORRIDOR_WIDTH, 3.0 * CORRIDOR_WIDTH], [1.0 * CORRIDOR_WIDTH, 1.0 * CORRIDOR_WIDTH], 'r')
        pyplot.plot([1.0 * CORRIDOR_WIDTH, 1.0 * CORRIDOR_WIDTH], [1.0 * CORRIDOR_WIDTH, 3.0 * CORRIDOR_WIDTH], 'r')
        for i in range(0, len(steps)):
            point = steps[i]
            x_1 = x1(point)
            y_1 = y1(point)
            x_2 = x2(point)
            y_2 = y2(point)
            x_3 = x3(point)
            y_3 = y3(point)
            x_4 = x4(point)
            y_4 = y4(point)

            temp_dx = x_1 - point[0]
            temp_dy = y_1 - point[1]
            if abs(temp_dx) < EPS:
                start_angle = pi / 2.0
            else:
                start_angle = arctan(temp_dy / temp_dx)
            temp_dx = x_4 - point[0]
            temp_dy = y_4 - point[1]
            if abs(temp_dx) < EPS:
                end_angle = 3.0 * pi / 2.0
            else:
                end_angle = pi + arctan(temp_dy / temp_dx)

            phis = linspace(start_angle, end_angle, 16)
            alpha = 0.3 + (i + 1) / len(steps) / 2
            for j in range(0, len(phis) - 1):
                pyplot.plot([point[0] + A_PARAM * cos(phis[j]), point[0] + A_PARAM * cos(phis[j + 1])],
                            [point[1] + A_PARAM * sin(phis[j]), point[1] + A_PARAM * sin(phis[j + 1])],
                            'b--', linewidth=1, alpha=alpha)

            pyplot.plot(point[0], point[1], 'bo', markersize=2, alpha=alpha)
            pyplot.plot([x_1, x_2], [y_1, y_2], 'b--', linewidth=1, alpha=alpha)
            pyplot.plot([x_2, x_3], [y_2, y_3], 'b--', linewidth=1, alpha=alpha)
            pyplot.plot([x_3, x_4], [y_3, y_4], 'b--', linewidth=1, alpha=alpha)

        pyplot.xlim([-CORRIDOR_WIDTH, 4.0 * CORRIDOR_WIDTH])
        pyplot.ylim([-CORRIDOR_WIDTH, 4.0 * CORRIDOR_WIDTH])

        w_coefficient = A_PARAM / CORRIDOR_WIDTH
        w_coefficient_str = '%.3lf' % w_coefficient
        title = 'a = %.1lf%% of corridor width' % (w_coefficient * 100)
        pyplot.xlabel('x')
        pyplot.ylabel('y')
        pyplot.title(title)
        if not os.path.exists(OUT_DIR):
            os.makedirs(OUT_DIR)
        pyplot.savefig(OUT_DIR + w_coefficient_str + 'w' + '.png')
        pyplot.show()
