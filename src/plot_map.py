# 2) load our new unified .json
import numpy as np
import matplotlib.pyplot as plt
import json
with open("../maps/online_data.json","r") as f:
    data = json.load(f)

# 3) plot the occupancy grid
plt.figure(figsize=(8,8))
world = np.array(data["occupancy"])
plt.imshow(world, cmap='gray', origin='lower')

# 4) plot tunnels as before …
# …

# 5) scatter start/goal
sx, sy = data["map"]["start"]["x"], data["map"]["start"]["y"]
gx, gy = data["map"]["goal" ]["x"], data["map"]["goal" ]["y"]
plt.scatter([sx],[sy],c="green",s=100,label="start")
plt.scatter([gx],[gy],c="red"  ,s=100,label="goal")

# 6) scatter your new intermediate goal
ix, iy = data["intermediate_goal"]["x"], data["intermediate_goal"]["y"]
plt.scatter([ix],[iy],c="blue", s=100, label="intermediate")

plt.legend()
plt.axis("equal")
plt.show()
