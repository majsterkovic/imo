import numpy as np
import pprint as pp
from matplotlib import pyplot as plt
import shutil
import os
import copy
from typing import List, Tuple, Dict, Any
from IPython.display import clear_output
import time
from tqdm.notebook import tqdm
import random
import pandas as pd
from tqdm import tqdm

def read_data_file(file_name: str) -> Dict[Any, Any]:
    tsp_data = {}
    with open(file_name, 'r') as file:
        for line in file:
            if line.startswith('EOF'):
                break
            if line.startswith('DIMENSION'):
                tsp_data['DIMENSION'] = int(line.split()[1])
            parts = line.split()
            if parts[0].isdigit():
                node, x, y = map(int, parts)
                tsp_data[node] = (x, y)

    return tsp_data

def calculate_distance_matrix(tsp_data: Dict[Any, Any]) -> np.ndarray:
    N = tsp_data['DIMENSION']
    tsp_data.pop('DIMENSION')

    distance_matrix = [[0 for _ in range(N + 1)] for _ in range(N + 1)]
    for node in tsp_data:
        x1, y1 = tsp_data[node]
        for other_node in tsp_data:
            if node == other_node:
                continue
            else:
                x2, y2 = tsp_data[other_node]
                distance = np.sqrt((x1 - x2)**2 + (y1 - y2)**2)
                distance_matrix[node][other_node] = distance
                distance_matrix[other_node][node] = distance
    return np.array(distance_matrix)

def plot_cities(data: Dict[Any, Any]) -> None:
    x = []
    y = []
    for city in data:
        x.append(data[city][0])
        y.append(data[city][1])
    plt.tight_layout()
    plt.scatter(x, y)
    plt.show()

def plot_cycles(cycle1: List, cycle2: List, tsp_data: Dict[Any, Any], file_name=None) -> None:
    plt.figure(figsize=(8, 6))

    for city in cycle1:
        plt.scatter(tsp_data[city][0], tsp_data[city][1], color='red', s=30)
        plt.text(tsp_data[city][0], tsp_data[city][1], str(city), fontsize=9, ha='left', va='bottom')

    cycle1_x = [tsp_data[city][0] for city in cycle1]
    cycle1_y = [tsp_data[city][1] for city in cycle1]
    cycle1_x.append(cycle1_x[0])
    cycle1_y.append(cycle1_y[0])
    plt.plot(cycle1_x, cycle1_y, linestyle='-', color='blue', label='Cycle 1')

    for city in cycle2:
        plt.scatter(tsp_data[city][0], tsp_data[city][1], color='red', s=30)
        plt.text(tsp_data[city][0], tsp_data[city][1], str(city), fontsize=9, ha='left', va='bottom')

    cycle2_x = [tsp_data[city][0] for city in cycle2]
    cycle2_y = [tsp_data[city][1] for city in cycle2]
    cycle2_x.append(cycle2_x[0])
    cycle2_y.append(cycle2_y[0])
    plt.plot(cycle2_x, cycle2_y, linestyle='-', color='green', label='Cycle 2')

    plt.title('Visualization of Cycles')
    plt.xlabel('X-coordinate')
    plt.ylabel('Y-coordinate')
    plt.grid(True)
    plt.legend()
    if file_name is not None:
        plt.savefig(file_name)
    plt.show()

def get_nodes(tsp_data: Dict[Any, Any]) -> np.ndarray:
    nodes = []
    for node in tsp_data:
        if node != 'DIMENSION':
            nodes.append(node)
    return np.array(nodes)

def calculate_cycles_length(cycle1: List, cycle2: List, distance_matrix: np.ndarray) -> float:
    c = [cycle1, cycle2]
    total_length = 0
    for cycle in c:
        length = 0
        for i in range(len(cycle)):
            length += distance_matrix[cycle[i-1]][cycle[i]]
        total_length += length

    return total_length