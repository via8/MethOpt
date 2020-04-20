import numpy
import matplotlib.pyplot
from mpl_toolkits.mplot3d import Axes3D
from pylab import meshgrid


A_PARAM = 4.0
B_PARAM = 6.0
C_PARAM = 3.0

# define f(x,y) function here
def f(x, y):
    return A_PARAM * x + y + 4.0 * numpy.sqrt(1.0 + B_PARAM * x * x + C_PARAM * y * y)

# define x = x(y, C) from f(x, y) = C here
def isoline(y, z, sign):
    w = y - z
    W = w * w
    D = numpy.sqrt(A_PARAM * A_PARAM * W - (16.0 * B_PARAM - A_PARAM * A_PARAM) * (16.0 + 16.0 * C_PARAM * y * y - W))
    return (A_PARAM * w + sign * D) / (16.0 * B_PARAM - A_PARAM * A_PARAM)


file = open('data.txt', 'r')
data = file.read()
file.close()

points = numpy.fromstring(data, sep='\n').reshape(-1, 2)

x = numpy.linspace(-abs(points[0][0]), abs(points[0][0]), 20)
y = numpy.linspace(-abs(points[0][1]), abs(points[0][1]), 20)
X, Y = meshgrid(x, y)
Z = f(X, Y)

points_x = []
points_y = []
points_z = []
for i in range(0, len(points)):
    points_x.append(points[i][0])
    points_y.append(points[i][1])
    points_z.append(f(points[i][0], points[i][1]))

fig = matplotlib.pyplot.figure()
ax = matplotlib.pyplot.axes(projection='3d')
surface = Axes3D.plot_surface(ax, X, Y, Z, alpha=0.5)

ax.plot(points_x, points_y, points_z, color='r')

n = 100
for step in range(1, min(4, len(points))):
    isoline_y = numpy.linspace(-abs(points[step][0]) - 2, abs(points[step][0]) + 2, n)

    isoline_z = []
    isoline_x = []
    for i in range(0, n):
        isoline_z.append(points_z[step])
        isoline_x.append(isoline(isoline_y[i], isoline_z[i], 1.0))
    ax.plot(isoline_x, isoline_y, isoline_z, color='g', linestyle='--')

    isoline_z = []
    isoline_x = []
    for i in range(0, n):
        isoline_z.append(points_z[step])
        isoline_x.append(isoline(isoline_y[i], isoline_z[i], -1.0))
    ax.plot(isoline_x, isoline_y, isoline_z, color='g', linestyle='--')

matplotlib.pyplot.show()
