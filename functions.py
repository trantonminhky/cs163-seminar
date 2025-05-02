from rtree import MBR, Node, RTree  # Import necessary classes
import sys

# MBR methods
def mbr_init(self, x_min=0, y_min=0, x_max=0, y_max=0):
    self.x_min = x_min
    self.y_min = y_min
    self.x_max = x_max
    self.y_max = y_max

def mbr_area(self):
    return (self.x_max - self.x_min) * (self.y_max - self.y_min)

def mbr_merge(self, other):
    return MBR(
        min(self.x_min, other.x_min),
        min(self.y_min, other.y_min),
        max(self.x_max, other.x_max),
        max(self.y_max, other.y_max)
    )

def mbr_contains(self, other):
    return (self.x_min <= other.x_min and self.y_min <= other.y_min and
            self.x_max >= other.x_max and self.y_max >= other.y_max)

def mbr_intersects(self, other):
    return not (self.x_max < other.x_min or self.x_min > other.x_max or
                self.y_max < other.y_min or self.y_min > other.y_max)

# Node methods
def node_init(self, is_leaf):
    self.is_leaf = is_leaf
    self.mbr = MBR()
    self.children = []

# RTree methods
def rtree_init(self, max_children):
    self.max_children = max_children
    self.root = Node(True)

def rtree_insert(self, mbr):
    self.insert_recursive(self.root, mbr, None)

def rtree_insert_recursive(self, node, mbr, parent):
    if node.is_leaf:
        if len(node.children) < self.max_children:
            new_node = Node(True)
            new_node.mbr = mbr
            node.children.append(new_node)
            self.update_mbr(node)
            if parent:
                self.update_mbr(parent)
        else:
            if not parent:  # Root case
                new_parent = Node(False)
                self.split_node(node, new_parent)
                self.root = new_parent
                target = self.choose_best_node(new_parent, mbr)
                self.insert_recursive(target, mbr, new_parent)
            else:  # Non-root case
                self.split_node(node, parent)
                target = self.choose_best_node(parent, mbr)
                self.insert_recursive(target, mbr, parent)
    else:
        target = self.choose_best_node(node, mbr)
        self.insert_recursive(target, mbr, node)

def rtree_choose_best_node(self, node, mbr):
    if not node.children:
        return node
    best = node.children[0]
    min_enlargement = (node.children[0].mbr.merge(mbr).area() - node.children[0].mbr.area())
    for child in node.children[1:]:
        enlargement = (child.mbr.merge(mbr).area() - child.mbr.area())
        if enlargement < min_enlargement:
            min_enlargement = enlargement
            best = child
    return best

def rtree_split_node(self, node, parent):
    median = len(node.children) // 2
    new_node = Node(node.is_leaf)
    
    new_node.children = node.children[median:]
    node.children = node.children[:median]
    
    self.update_mbr(node)
    self.update_mbr(new_node)
    
    if not parent.children:
        parent.children.append(node)
        parent.children.append(new_node)
    else:
        for i, child in enumerate(parent.children):
            if child == node:
                parent.children[i] = node
                parent.children.insert(i + 1, new_node)
                break
    self.update_mbr(parent)

def rtree_update_mbr(self, node):
    if not node.children:
        return
    node.mbr = node.children[0].mbr
    for child in node.children[1:]:
        node.mbr = node.mbr.merge(child.mbr)

def rtree_search(self, search_mbr):
    results = []
    self.search_recursive(self.root, search_mbr, results)
    if not results:
        print("No MBR found in the search range.")
    else:
        for node in results:
            print(f"Found MBR: ({node.mbr.x_min}, {node.mbr.y_min}, "
                  f"{node.mbr.x_max}, {node.mbr.y_max})")

def rtree_search_recursive(self, node, search_mbr, results):
    if node.is_leaf:
        for child in node.children:
            if child.mbr.intersects(search_mbr):
                results.append(child)
    else:
        for child in node.children:
            if child.mbr.intersects(search_mbr):
                self.search_recursive(child, search_mbr, results)

def rtree_find_area(self):
    if not self.root.children:
        print("Tree is empty.")
        return
    print("Areas of all MBRs in the R-tree:")
    self.find_area_recursive(self.root, 0)

def rtree_find_area_recursive(self, node, level):
    indent = "  " * level
    print(f"{indent}{'Leaf' if node.is_leaf else 'Internal'} Node MBR "
          f"({node.mbr.x_min}, {node.mbr.y_min}, {node.mbr.x_max}, {node.mbr.y_max}): "
          f"{node.mbr.area()}")
    
    if node.is_leaf:
        for i, child in enumerate(node.children, 1):
            print(f"{indent}  Child {i} MBR "
                  f"({child.mbr.x_min}, {child.mbr.y_min}, {child.mbr.x_max}, {child.mbr.y_max}): "
                  f"{child.mbr.area()}")
    else:
        for child in node.children:
            self.find_area_recursive(child, level + 1)

def rtree_display_tree(self):
    if not self.root.children:
        print("Tree is empty.")
        return
    print("R-tree Structure:")
    self.display_tree_recursive(self.root, 0)

def rtree_display_tree_recursive(self, node, level):
    indent = "  " * level
    print(f"{indent}{'Leaf' if node.is_leaf else 'Internal'} Node "
          f"MBR: ({node.mbr.x_min}, {node.mbr.y_min}, {node.mbr.x_max}, {node.mbr.y_max})")
    
    if node.is_leaf:
        for i, child in enumerate(node.children, 1):
            print(f"{indent}  Child {i}: "
                  f"({child.mbr.x_min}, {child.mbr.y_min}, {child.mbr.x_max}, {child.mbr.y_max})")
    else:
        for child in node.children:
            self.display_tree_recursive(child, level + 1)