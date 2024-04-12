#include <iostream>

template <typename T = int>
class RB_Node {
public:
    T value;
    RB_Node* left;
    RB_Node* right;
    RB_Node* parent;
    bool red;

    explicit RB_Node(T value)
        : value(value)
        , left(nullptr)
        , right(nullptr)
        , parent(nullptr)
        , red(true)
    {
    }
};

int main()
{
    auto root = new RB_Node<int>(1);
    root->left = new RB_Node<int>(2);
    root->right = new RB_Node<int>(3);

    std::cout << root->value << std::endl;

    delete root->left;
    delete root->right;
    delete root;
    return 0;
}