import numpy as np
import matplotlib.pyplot as plt
def build_environment(gridsize = 999): 
    world = np.zeros((gridsize, gridsize)) 
    start_x, start_y = 25, gridsize//2 
    goal_x, goal_y = gridsize - 25, gridsize//2 
    intermdiate_goal_zone = [200, 400] 
    constraint_zone = [401, 800] 
    tunnel_width = 5 
    d = 9 
    num_tunnels = gridsize//9 
    world[:, constraint_zone[0]:constraint_zone[1]] = 1
    return world, start_x, start_y, goal_x, goal_y, intermdiate_goal_zone, constraint_zone, tunnel_width, d, num_tunnels

def plot_environment(world, start_x, start_y, goal_x, goal_y, intermdiate_goal_zone, constraint_zone):
    plt.imshow(world, cmap='gray', origin='lower')
    plt.scatter(start_x, start_y, c='green', label='Start', s=100)
    plt.scatter(goal_x, goal_y, c='red', label='Goal', s=100)
    plt.axvspan(constraint_zone[0], constraint_zone[1], color='blue', alpha=0.3, label='Constraint Zone')
    plt.axvspan(intermdiate_goal_zone[0], intermdiate_goal_zone[1], color='orange', alpha=0.3, label='Intermediate Goal Zone')
    plt.legend()
    plt.title('Environment')
    plt.xlabel('X-axis')
    plt.ylabel('Y-axis')
    plt.show()

world, start_x, start_y, goal_x, goal_y, intermdiate_goal_zone, constraint_zone, tunnel_width, d, num_tunnels = build_environment()
plot_environment(world, start_x, start_y, goal_x, goal_y, intermdiate_goal_zone, constraint_zone)