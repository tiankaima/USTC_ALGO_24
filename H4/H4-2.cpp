#include <iostream>
#include <limits>

template <typename T>
class Binary_Node {
public:
    T value_;
    Binary_Node* left_child_;
    Binary_Node* right_child_;
    Binary_Node* parent_;

    explicit Binary_Node(T value)
        : value_(value)
        , left_child_(nullptr)
        , right_child_(nullptr)
        , parent_(nullptr)
    {
    }

    Binary_Node* min()
    {
        auto node = this;
        while (node->left_child_ != nullptr) {
            node = node->left_child_;
        }

        return node;
    }

    Binary_Node* max()
    {
        auto node = this;
        while (node->right_child_ != nullptr) {
            node = node->right_child_;
        }

        return node;
    }

    Binary_Node* successor()
    {
        if (right_child_ != nullptr) {
            return right_child_->min();
        }

        auto node = this;
        auto parent = node->parent_;
        while (parent != nullptr && node == parent->right_child_) {
            node = parent;
            parent = parent->parent_;
        }

        return parent;
    }

    Binary_Node* predecessor()
    {
        if (left_child_ != nullptr) {
            return left_child_->max();
        }

        auto node = this;
        auto parent = node->parent_;
        while (parent != nullptr && node == parent->left_child_) {
            node = parent;
            parent = parent->parent_;
        }

        return parent;
    }

    Binary_Node* insert(T value)
    {
        auto node = this;
        auto node_p = this->parent_;
        auto new_node = new Binary_Node(value);
        while (node != nullptr) {
            node_p = node;
            if (value < node->value_) {
                node = node->left_child_;
            } else {
                node = node->right_child_;
            }
        }
        new_node->parent_ = node_p;
        if (node_p == nullptr) {
            this->value_ = value; // empty tree, set root value
        } else if (value < node_p->value_) {
            node_p->left_child_ = new_node;
        } else {
            node_p->right_child_ = new_node;
        }
        return new_node;
    }

    void remove_self() // NOLINT(*-no-recursion)
    {
        if (left_child_ == nullptr && right_child_ == nullptr) {
            if (parent_ != nullptr && parent_->left_child_ == this) {
                parent_->left_child_ = nullptr;
            }
            if (parent_ != nullptr && parent_->right_child_ == this) {
                parent_->right_child_ = nullptr;
            }
        }
        if (left_child_ == nullptr && right_child_ != nullptr) {
            if (parent_ != nullptr && parent_->left_child_ == this) {
                parent_->left_child_ = right_child_;
            }
            if (parent_ != nullptr && parent_->right_child_ == this) {
                parent_->right_child_ = right_child_;
            }
            right_child_->parent_ = parent_;
        }
        if (left_child_ != nullptr && right_child_ == nullptr) {
            if (parent_ != nullptr && parent_->left_child_ == this) {
                parent_->left_child_ = left_child_;
            }
            if (parent_ != nullptr && parent_->right_child_ == this) {
                parent_->right_child_ = left_child_;
            }
            left_child_->parent_ = parent_;
        }
        if (left_child_ != nullptr && right_child_ != nullptr) {
            auto successor = this->successor();
            value_ = successor->value_;
            successor->remove_self();
        }
    }

    void free() // NOLINT(*-no-recursion)
    {
        if (left_child_ != nullptr) {
            left_child_->free();
        }
        if (right_child_ != nullptr) {
            right_child_->free();
        }
        delete this;
    }
};

typedef struct {
    int a;
    int b;
} Pair;

bool operator<(const Pair& lhs, const Pair& rhs)
{
    return lhs.a < rhs.a;
}

int main()
{
    int n;
    scanf("%d", &n);

    Binary_Node<Pair>* root = nullptr;
    for (int i = 0; i < n; i++) {
        int a, b;
        scanf("%d %d", &a, &b);

        auto p = Pair { a, b };
        if (root == nullptr) {
            root = new Binary_Node<Pair>(p);
            printf("0\n");
            continue;
        }

        auto p_node = root->insert(p);

        auto predecessor = p_node->predecessor();
        auto successor = p_node->successor();

        int p_b = -1, s_a = std::numeric_limits<int>::max();
        if (predecessor != nullptr) {
            p_b = predecessor->value_.b;
        }
        if (successor != nullptr) {
            s_a = successor->value_.a;
        }

        if (p_b > p.a || s_a < p.b) {
            printf("-1\n");
            p_node->remove_self();
        } else {
            printf("0\n");
        }
    }

    if (root != nullptr)
        root->free();
    return 0;
}