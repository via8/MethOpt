import numpy
import matplotlib.pyplot
from mpl_toolkits.mplot3d import Axes3D
from pylab import meshgrid


A_PARAM = 4
B_PARAM = 6
C_PARAM = 3


def f(x, y):
    return A_PARAM * x + y + 4.0 * numpy.sqrt(1.0 + B_PARAM * x * x + C_PARAM * y * y)


x = numpy.linspace(-5.0, 5.0, 100)
y = numpy.linspace(-5.0, 5.0, 100)
X, Y = meshgrid(x, y)
Z = f(X, Y)

fig = matplotlib.pyplot.figure()
ax = matplotlib.pyplot.axes(projection='3d')
surface = Axes3D.plot_surface(ax, X, Y, Z)

matplotlib.pyplot.show()
