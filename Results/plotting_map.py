# CTMP_MANI/Results/plotting_map.py
import os
import json
import numpy as np
import matplotlib.pyplot as plt

# 1) load the map
map_path = os.path.join(os.getcwd(), 'maps', 'map.txt')
world   = np.loadtxt(map_path, dtype=int)

# 2) load the tunnel/path data
tunnel_path = os.path.join(os.getcwd(), 'data', 'test_tunnels.json')
with open(tunnel_path, 'r') as f:
    data = json.load(f)

# 3) plot the occupancy grid
plt.figure(figsize=(8,8))
plt.imshow(world, cmap='gray', origin='lower')
plt.title("Map + Precomputed Path")
plt.xlabel("x")
plt.ylabel("y")

# 4) overlay each “tunnel” (i.e. the saved path)
# for t in data["tunnels"]:
#     xs = [p["x"] for p in t["points"]]
#     ys = [p["y"] for p in t["points"]]
#     # plt.plot(xs, ys, '-o', label=f"tunnel {t['id']}")
#     plt.plot(xs, ys, '-o')

for i, P in enumerate(data.get("paths", [])):
    xs = [pt["x"] for pt in P["points"]]
    ys = [pt["y"] for pt in P["points"]]
    label = "precomputed paths" if i==0 else None
    plt.plot(xs, ys, '-r', linewidth=0.5, label=label)


# for P in data.get("paths", []):
#     xs = [pt["x"] for pt in P["points"]]
#     ys = [pt["y"] for pt in P["points"]]
#     # pick a different colour or linestyle so you can tell tunnels & paths apart
#     plt.plot(xs, ys, '-r', linewidth=2, label=f"path {P['id']}")

# 5) scatter start/goal
sx, sy = data["map"]["start"]["x"], data["map"]["start"]["y"]
gx, gy = data["map"]["goal"]["x"],  data["map"]["goal"]["y"]
plt.scatter([sx], [sy], c="green",  s=100, label="start")
plt.scatter([gx], [gy], c="red",    s=100, label="goal")

plt.legend()
plt.axis("equal")
plt.show()
