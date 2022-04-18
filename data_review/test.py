import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation

df = pd.read_csv (r'D1.csv')
lat = df.loc[:,"latitude"].values
lon = df.loc[:,"longitude"].values
alt = df.loc[:,"altitude"].values

# num = 1
# print(lat[:num*10])

def update_lines(num):
    text.set_text("{:d}: [{:.4f},{:.4f},{:.4f}]".format(num, lat[num], lon[num], alt[num]))  # for debugging
    x.append(lat[num])
    y.append(lon[num])
    z.append(alt[num])
    graph._offsets3d = (x, y, z)
    return graph,

x = [0]
y = [0]
z = [0]

fig = plt.figure(figsize=(5, 5))
ax = fig.add_subplot(111, projection="3d")
graph = ax.scatter(x, y, z, color='blue')
text = fig.text(0, 1, "TEXT", va='top')  # for debugging

ax.set_ylabel('latitude')
ax.set_xlabel('longitude')
ax.set_zlabel('altitude (m)')

ax.set_yticklabels([])
ax.set_xticklabels([])
# ax.set_zticklabels([])

plt.title("GLDRR Controlled Flight Test")

ax.set_xlim3d(35.7860, 35.7890)
ax.set_ylim3d(-115.2610, -115.258)
ax.set_zlim3d(-2, 240)

# Creating the Animation object
ani = animation.FuncAnimation(fig, update_lines, frames = len(lat), interval=len(lat), blit=False)
# ani.save('gldrr_flight.gif', writer='imagemagick', fps=30)
plt.show()