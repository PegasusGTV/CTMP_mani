# import json
# import matplotlib.pyplot as plt
# import matplotlib.animation as animation

# # Load path
# with open('../data/final_path.json') as f:
#     path = json.load(f)

# x = [p["x"] for p in path]
# y = [p["y"] for p in path]

# fig, ax = plt.subplots()
# line, = ax.plot([], [], 'bo-', lw=2)
# point, = ax.plot([], [], 'ro', markersize=8)

# def init():
#     ax.set_xlim(min(x)-10, max(x)+10)
#     ax.set_ylim(min(y)-10, max(y)+10)
#     return line, point

# def update(i):
#     line.set_data(x[:i+1], y[:i+1])
#     point.set_data(x[i], y[i])
#     return line, point

# ani = animation.FuncAnimation(fig, update, frames=len(x), init_func=init, blit=True, interval=50)
# plt.title("Path Animation")
# plt.xlabel("X")
# plt.ylabel("Y")
# plt.grid()
# plt.show()




# import matplotlib
# matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
# from matplotlib.animation import FFMpegWriter
import json

# 1. Load map and path data
with open("../maps/online_data.json", "r") as f:
    data = json.load(f)

with open("../data/final_path.json", "r") as f:
    path = json.load(f)


# 2. Extract occupancy grid
world = np.array(data["occupancy"])

# 3. Extract key positions
sx, sy = data["map"]["start"]["x"], data["map"]["start"]["y"]
gx, gy = data["map"]["goal"]["x"], data["map"]["goal"]["y"]
ix, iy = data["intermediate_goal"]["x"], data["intermediate_goal"]["y"]

# 4. Extract path coordinates
px = [p["x"] for p in path]
py = [p["y"] for p in path]

# 5. Setup figure
fig, ax = plt.subplots(figsize=(8, 8))
ax.imshow(world, cmap='gray', origin='lower')
ax.scatter([sx], [sy], c='green', s=100, label='Start')
ax.scatter([gx], [gy], c='red', s=100, label='Goal')
ax.scatter([ix], [iy], c='blue', s=100, label='Intermediate Goal')
line, = ax.plot([], [], 'bo-', lw=2)
point, = ax.plot([], [], 'ro', markersize=6)

ax.legend()
ax.set_title("Robot Path Animation")
ax.set_xlim(0, world.shape[1])
ax.set_ylim(0, world.shape[0])
ax.set_aspect("equal")

# 6. Animation functions
def init():
    line.set_data([], [])
    point.set_data([], [])
    return line, point

def update(i):
    line.set_data([px[:i+1]], [py[:i+1]])
    point.set_data([px[i]], [py[i]])
    return line, point


ani = animation.FuncAnimation(fig, update, frames=len(px), init_func=init, blit=True, interval=10)

plt.show()

# writer = FFMpegWriter(fps=30, metadata=dict(artist='Gopal'), bitrate=1800)

ani.save("../Results/path_animation_map_1.gif", writer='pillow', dpi=150)


