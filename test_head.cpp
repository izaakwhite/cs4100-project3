#include "parse_tree.h"

using namespace std;

int main() {
    TreeNode* root = new TreeNode("root");

    // Create first branch
    TreeNode* A1 = new TreeNode("A1");
    A1->addChild(new TreeNode("B1"));
    A1->addChild(new TreeNode("B2"));
    A1->addChild(new TreeNode("B3"));

    // Create the second branch
    TreeNode* A2 = new TreeNode("A2");
    A2->addChild(new TreeNode("C1"));
    A2->addChild(new TreeNode("C2"));
    A2->addChild(new TreeNode("C3"));

    root->addChild(A1);
    root->addChild(A2);
    printTree(root);
    return 0;
}