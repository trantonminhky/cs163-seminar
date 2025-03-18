#include <iostream>
#include <algorithm>
#include "RTree.h"

// MBR methods (unchanged)
MBR::MBR() : x_min(0), y_min(0), x_max(0), y_max(0) {}
MBR::MBR(int x_min, int y_min, int x_max, int y_max) : x_min(x_min), y_min(y_min), x_max(x_max), y_max(y_max) {}
int MBR::area() const { return (x_max - x_min) * (y_max - y_min); }
MBR MBR::merge(const MBR& other) const {
    return MBR(std::min(x_min, other.x_min), std::min(y_min, other.y_min),
        std::max(x_max, other.x_max), std::max(y_max, other.y_max));
}
bool MBR::contains(const MBR& other) const {
    return x_min <= other.x_min && y_min <= other.y_min && x_max >= other.x_max && y_max >= other.y_max;
}
bool MBR::intersects(const MBR& other) const {
    return !(x_max < other.x_min || x_min > other.x_max || y_max < other.y_min || y_min > other.y_max);
}

// Node constructor (unchanged)
Node::Node(bool is_leaf) : is_leaf(is_leaf) {}

// RTree methods
RTree::RTree(int max_children) : max_children(max_children) {
    root = new Node(true);
}

RTree::~RTree() {
    delete_node(root);
}

void RTree::delete_node(Node* node) {
    if (!node) return;
    for (Node* child : node->children) {
        delete_node(child);
    }
    delete node;
}

void RTree::insert(const MBR& mbr) {
    insert_recursive(root, mbr, nullptr);
}

void RTree::insert_recursive(Node* node, const MBR& mbr, Node* parent) {
    if (node->is_leaf) {
        if (node->children.size() < max_children) {
            Node* new_node = new Node(true);
            new_node->mbr = mbr;
            node->children.push_back(new_node);
            update_mbr(node);
            if (parent) update_mbr(parent);
        }
        else {
            if (!parent) { // Root case
                Node* new_parent = new Node(false);
                split_node(node, new_parent);
                root = new_parent;
                Node* target = choose_best_node(new_parent, mbr);
                insert_recursive(target, mbr, new_parent);
            }
            else { // Non-root case
                split_node(node, parent);
                Node* target = choose_best_node(parent, mbr);
                insert_recursive(target, mbr, parent);
            }
        }
    }
    else {
        Node* target = choose_best_node(node, mbr);
        insert_recursive(target, mbr, node);
    }
}

Node* RTree::choose_best_node(Node* node, const MBR& mbr) {
    if (node->children.empty()) return node;
    Node* best = node->children[0];
    int min_enlargement = (node->children[0]->mbr.merge(mbr)).area() - node->children[0]->mbr.area();
    for (size_t i = 1; i < node->children.size(); ++i) {
        int enlargement = (node->children[i]->mbr.merge(mbr)).area() - node->children[i]->mbr.area();
        if (enlargement < min_enlargement) {
            min_enlargement = enlargement;
            best = node->children[i];
        }
    }
    return best;
}

void RTree::split_node(Node* node, Node* parent) {
    int median = node->children.size() / 2;
    Node* new_node = new Node(node->is_leaf);

    new_node->children = std::vector<Node*>(node->children.begin() + median, node->children.end());
    node->children = std::vector<Node*>(node->children.begin(), node->children.begin() + median);

    update_mbr(node);
    update_mbr(new_node);

    if (parent->children.empty()) {
        parent->children.push_back(node);
        parent->children.push_back(new_node);
    }
    else {
        for (size_t i = 0; i < parent->children.size(); ++i) {
            if (parent->children[i] == node) {
                parent->children[i] = node;
                parent->children.insert(parent->children.begin() + i + 1, new_node);
                break;
            }
        }
    }
    update_mbr(parent);
}

void RTree::update_mbr(Node* node) {
    if (node->children.empty()) return;
    node->mbr = node->children[0]->mbr;
    for (size_t i = 1; i < node->children.size(); ++i) {
        node->mbr = node->mbr.merge(node->children[i]->mbr);
    }
}

void RTree::search(const MBR& search_mbr) {
    std::vector<Node*> results;
    search_recursive(root, search_mbr, results);
    if (results.empty()) {
        std::cout << "No MBR found in the search range." << std::endl;
    }
    else {
        for (const Node* node : results) {
            std::cout << "Found MBR: (" << node->mbr.x_min << ", " << node->mbr.y_min
                << ", " << node->mbr.x_max << ", " << node->mbr.y_max << ")\n";
        }
    }
}

void RTree::search_recursive(Node* node, const MBR& search_mbr, std::vector<Node*>& results) {
    if (node->is_leaf) {
        for (Node* child : node->children) {
            if (child->mbr.intersects(search_mbr)) {
                results.push_back(child);
            }
        }
    }
    else {
        for (Node* child : node->children) {
            if (child->mbr.intersects(search_mbr)) {
                search_recursive(child, search_mbr, results);
            }
        }
    }
}

void RTree::find_area() {
    if (root->children.empty()) {
        std::cout << "Tree is empty." << std::endl;
        return;
    }
    std::cout << "Areas of all MBRs in the R-tree:\n";
    find_area_recursive(root, 0);
}

void RTree::find_area_recursive(Node* node, int level) {
    std::string indent(level * 2, ' '); // Indentation based on level

    // Print the current node's MBR area
    std::cout << indent << (node->is_leaf ? "Leaf" : "Internal") << " Node MBR ("
        << node->mbr.x_min << ", " << node->mbr.y_min << ", "
        << node->mbr.x_max << ", " << node->mbr.y_max << "): "
        << node->mbr.area() << std::endl;

    // If leaf, print areas of all child MBRs
    if (node->is_leaf) {
        for (size_t i = 0; i < node->children.size(); ++i) {
            Node* child = node->children[i];
            std::cout << indent << "  Child " << i + 1 << " MBR ("
                << child->mbr.x_min << ", " << child->mbr.y_min << ", "
                << child->mbr.x_max << ", " << child->mbr.y_max << "): "
                << child->mbr.area() << std::endl;
        }
    }
    // If internal, recurse into children
    else {
        for (Node* child : node->children) {
            find_area_recursive(child, level + 1);
        }
    }
}

void RTree::display_tree() {
    if (root->children.empty()) {
        std::cout << "Tree is empty." << std::endl;
        return;
    }
    std::cout << "R-tree Structure:\n";
    display_tree_recursive(root, 0);
}

void RTree::display_tree_recursive(Node* node, int level) {
    // Indentation based on level
    std::string indent(level * 2, ' ');

    // Print node info
    std::cout << indent << (node->is_leaf ? "Leaf" : "Internal") << " Node "
        << "MBR: (" << node->mbr.x_min << ", " << node->mbr.y_min << ", "
        << node->mbr.x_max << ", " << node->mbr.y_max << ")\n";

    // Print children
    if (node->is_leaf) {
        for (size_t i = 0; i < node->children.size(); ++i) {
            Node* child = node->children[i];
            std::cout << indent << "  Child " << i + 1 << ": ("
                << child->mbr.x_min << ", " << child->mbr.y_min << ", "
                << child->mbr.x_max << ", " << child->mbr.y_max << ")\n";
        }
    }
    else {
        for (size_t i = 0; i < node->children.size(); ++i) {
            display_tree_recursive(node->children[i], level + 1);
        }
    }
}