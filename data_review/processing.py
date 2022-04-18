import pandas as pd
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

df = pd.read_csv (r'D1.csv')
lat = df.loc[:,"latitude"]
lon = df.loc[:,"longitude"]
alt = df.loc[:,"altitude"]

print(min(lat), max(lat))
print(min(lon), max(lon))
print(min(alt), max(alt))

# plt.ion()

# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# sc = ax.scatter(lat[0], lon[0], alt[0])
# fig.show()

# for i in range(0, 400):
#     plt.pause(0.01)
#     # print(lat[i], lon[i], alt[i])
#     sc._offsets3d = (lat[i], lon[i], alt[i])
#     plt.draw()


# plt.show()

# from matplotlib import pyplot as plt
# import numpy as np
# from mpl_toolkits.mplot3d import Axes3D
# from matplotlib import animation

# fig = plt.figure()
# ax = fig.add_subplot(projection='3d')

# def gen(n):
#     phi = 0
#     while phi < 2*np.pi:
#         yield np.array([np.cos(phi), np.sin(phi), phi])
#         phi += 2*np.pi/n

# def update(num, data, line):
#     line.set_data(data[:2, :num])
#     line.set_3d_properties(data[2, :num])

# N = 100
# data = np.array(list(gen(N))).T
# line, = ax.plot(data[0, 0:1], data[1, 0:1], data[2, 0:1])

# # Setting the axes properties
# ax.set_xlim3d([-1.0, 1.0])
# ax.set_xlabel('X')

# ax.set_ylim3d([-1.0, 1.0])
# ax.set_ylabel('Y')

# ax.set_zlim3d([0.0, 10.0])
# ax.set_zlabel('Z')

# ani = animation.FuncAnimation(fig, update, N, fargs=(data, line), interval=10000/N, blit=False)
# #ani.save('matplot003.gif', writer='imagemagick')
# plt.show()