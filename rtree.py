class MBR:
    def __init__(self, x_min=0, y_min=0, x_max=0, y_max=0):
        self.x_min = x_min
        self.y_min = y_min
        self.x_max = x_max
        self.y_max = y_max
    
    def area(self):
        return (self.x_max - self.x_min) * (self.y_max - self.y_min)
    
    def merge(self, other):
        return MBR(
            min(self.x_min, other.x_min),
            min(self.y_min, other.y_min),
            max(self.x_max, other.x_max),
            max(self.y_max, other.y_max)
        )
    
    def contains(self, other):
        return (self.x_min <= other.x_min and self.y_min <= other.y_min and
                self.x_max >= other.x_max and self.y_max >= other.y_max)
    
    def intersects(self, other):
        return not (self.x_max < other.x_min or self.x_min > other.x_max or
                    self.y_max < other.y_min or self.y_min > other.y_max)

class Node:
    def __init__(self, is_leaf=False):
        self.is_leaf = is_leaf
        self.mbr = MBR()
        self.children = []

class RTree:
    def __init__(self, max_children=4):
        self.max_children = max_children
        self.root = Node(True)
    
    def insert(self, mbr):
        pass  # Implemented in functions.py
    
    def search(self, search_mbr):
        pass  # Implemented in functions.py
    
    def find_area(self):
        pass  # Implemented in functions.py
    
    def display_tree(self):
        pass  # Implemented in functions.py
    
    def insert_recursive(self, node, mbr, parent=None):
        pass  # Implemented in functions.py
    
    def choose_best_node(self, node, mbr):
        pass  # Implemented in functions.py
    
    def split_node(self, node, parent):
        pass  # Implemented in functions.py
    
    def update_mbr(self, node):
        pass  # Implemented in functions.py
    
    def search_recursive(self, node, search_mbr, results):
        pass  # Implemented in functions.py
    
    def find_area_recursive(self, node, level):
        pass  # Implemented in functions.py
    
    def display_tree_recursive(self, node, level=0):
        pass  # Implemented in functions.py