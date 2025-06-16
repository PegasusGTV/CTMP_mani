import numpy as np
import matplotlib.pyplot as plt
def build_environment(gridsize = 999): 
    world = np.zeros((gridsize, gridsize)) 
    start_x, start_y = 2, gridsize//2 
    goal_x, goal_y = gridsize - 3, gridsize//2 
    intermdiate_goal_zone = [200, 400] 
    constraint_zone = [401, 800] 
    tunnel_width = 5 
    d = 9 
    num_tunnels = gridsize//9 
    world[constraint_zone[0]:constraint_zone[1], :] = 1
    return world, start_x, start_y, goal_x, goal_y, intermdiate_goal_zone, constraint_zone, tunnel_width, d, num_tunnels