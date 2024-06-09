from utils import *

def gen_random_cycles(nodes=None, data=None) -> Tuple[np.ndarray, np.ndarray]:
    if data is not None:
        nodes = get_nodes(data)
    if nodes is None:
        raise ValueError('nodes and data cannot be None at the same time')
    nodes = copy.deepcopy(nodes)
    np.random.shuffle(nodes)
    half = len(nodes) // 2
    cycle1 = np.array(nodes[:half])
    cycle2 = np.array(nodes[half:])
    
    return cycle1, cycle2