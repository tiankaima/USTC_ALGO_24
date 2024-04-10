#include <iostream>

template <typename T = int>
class RB_Node {
public:
    T value;
    RB_Node* left;
    RB_Node* right;
    RB_Node* parent;
    bool red;

    RB_Node(T value)
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
    auto root = new RB_Node(1);
    root->left = new RB_Node(2);
    root->right = new RB_Node(3);

    std::cout << root->value << std::endl;
    return 0;
}