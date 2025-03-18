#ifndef TREE_H
#define TREE_H

#include <vector>

struct MBR {
    int x_min, y_min, x_max, y_max;
    MBR();
    MBR(int x_min, int y_min, int x_max, int y_max);
    int area() const;
    MBR merge(const MBR& other) const;
    bool contains(const MBR& other) const;
    bool intersects(const MBR& other) const;
};

struct Node {
    bool is_leaf;
    MBR mbr;
    std::vector<Node*> children;
    Node(bool is_leaf = false);
};

struct RTree {
    Node* root;
    int max_children;

    RTree(int max_children = 4);
    ~RTree();
    void insert(const MBR& mbr);
    void search(const MBR& search_mbr);
    void find_area();
    void display_tree(); // New public method

private:
    void insert_recursive(Node* node, const MBR& mbr, Node* parent = nullptr);
    Node* choose_best_node(Node* node, const MBR& mbr);
    void split_node(Node* node, Node* parent);
    void update_mbr(Node* node);
    void search_recursive(Node* node, const MBR& search_mbr, std::vector<Node*>& results);
    void find_area_recursive(Node* node, int level);
    void delete_node(Node* node);
    void display_tree_recursive(Node* node, int level = 0); // New private helper
};

#endif