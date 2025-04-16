#include "tree_node.h"
#include <iostream>

void printTree(TreeNode* node) {
    if (!node) return;
    std::cout << node->name;
    if (!node->children.empty()) {
        std::cout << " [";
        for (size_t i = 0; i < node->children.size(); i++) {
            printTree(node->children[i]);
            if (i < node->children.size() - 1)
                std::cout << ", ";
        }
        std::cout << "]";
    }
}
