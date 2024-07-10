#include <future>
#include <iostream>
#include <random>
#include <functional>

// 树节点结构体
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 打印树
void printTree(TreeNode* root, int depth = 0) {
    if (root == nullptr) return;
    printTree(root->right, depth + 1);
    for (int i = 0; i < depth; ++i) std::cout << "    ";
    std::cout << root->val << std::endl;
    printTree(root->left, depth + 1);
}

class Solution {
public:
    TreeNode* invertSubtrees(TreeNode* node) {
        if (node == nullptr)
            return node;

        // 模拟罕见条件，例如释放内存
        // if (shouldFreeMemory()) {
        //     std::cout << "Freeing left child of node with value: " << node->val << std::endl;
        //     delete node->left;
        //     node->left = nullptr;
        // }

        auto new_left = std::async(&Solution::invertSubtrees, this, node->right);
        auto new_right = std::async(&Solution::invertSubtrees, this, node->left);

        node->left = new_left.get();
        node->right = new_right.get();

        return node;
    }

    TreeNode* invertTree(TreeNode* root) {
        return invertSubtrees(root);
    }

private:
    bool shouldFreeMemory() {
        // 使用随机数生成器来模拟罕见的条件
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(1, 100);
        return dis(gen) == 1; // 1%的概率满足条件
    }
};

// 示例测试代码
int main() {
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    std::cout << "Original tree:" << std::endl;
    printTree(root);

    Solution solution;
    TreeNode* invertedRoot = solution.invertTree(root);

    std::cout << "\nInverted tree:" << std::endl;
    printTree(invertedRoot);

    // 清理内存
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}
