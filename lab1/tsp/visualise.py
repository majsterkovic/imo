import matplotlib.pyplot as plt
import numpy as np

def visualize_cycles(data, cycles):
    plt.figure(figsize=(18, 16))

    for node, (x, y) in data.items():
        plt.scatter(x, y, color='red', s=30)
        plt.text(x, y, str(node), fontsize=8, ha='left', va='bottom')

    colors = ['blue', 'green', 'orange', 'purple', 'cyan']
    for i, cycle in enumerate(cycles):
        cycle_x = [data[node][0] for node in cycle]
        cycle_y = [data[node][1] for node in cycle]
        cycle_x.append(cycle_x[0])
        cycle_y.append(cycle_y[0])
        plt.plot(cycle_x, cycle_y, linestyle='-', color=colors[i % len(colors)])

    plt.title('Visualization of Cycles')
    plt.xlabel('X-coordinate')
    plt.ylabel('Y-coordinate')
    plt.grid(True)
    # plt.show()
    plt.savefig("A_regret_heuristic.png")

if __name__ == "__main__":

    with open('cycle1.txt', 'r') as file:
        cycle1 = [int(node) for node in file.readline().split()]

    with open('cycle2.txt', 'r') as file:
        cycle2 = [int(node) for node in file.readline().split()]

    all_cycles = [cycle1, cycle2]

    data = {}
    with open('kroA200.tsp', 'r') as file:
        for line in file:
            if line.startswith('EOF'):
                break
            parts = line.split()
            if parts[0].isdigit():
                node, x, y = map(int, parts)
                data[node] = (x, y)

    print(data)
    visualize_cycles(data, all_cycles)