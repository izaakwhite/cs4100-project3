#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <vector>
#include <string>

class TreeNode {
public:
    std::string name;
    int weight;
    std::vector<TreeNode*> children;

    TreeNode(const std::string &name, int weight = 0) : name(name), weight(weight) {}
    void addChild(TreeNode* child) {
        children.push_back(child);
    }
    ~TreeNode() {
        for (auto child : children) {
            delete child;
        }
    }
};

#endif
