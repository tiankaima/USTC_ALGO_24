#include "iostream"

template <typename T = int>
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

    enum class WalkMode {
        PreOrder,
        InOrder,
        PostOrder
    };

    void walk( // NOLINT(*-no-recursion)
        WalkMode mode = WalkMode::InOrder,
        void (*func)(Binary_Node*) = [](Binary_Node<T>* node) { //
            std::cout << node->value_ << "\t"; //
        })
    {
        if (mode == WalkMode::PreOrder)
            func(this);

        if (left_child_ != nullptr)
            left_child_->walk(mode, func);

        if (mode == WalkMode::InOrder)
            func(this);

        if (right_child_ != nullptr)
            right_child_->walk(mode, func);

        if (mode == WalkMode::PostOrder)
            func(this);
    }

    void print(int depth = 0) // NOLINT(*-no-recursion)
    {
        std::cout << value_ << "→   ";

        if (right_child_ != nullptr)
            right_child_->print(depth + 1);

        std::cout << std::endl;
        for (int i = 0; i < depth; i++)
            std::cout << "    ";
        std::cout << " ↳   ";

        if (left_child_ != nullptr)
            left_child_->print(depth + 1);

        if (depth == 0)
            std::cout << std::endl;
    }

    Binary_Node* search(const T& value) // NOLINT(*-no-recursion)
    {
        if (this->value_ == value) {
            return this;
        }

        if (value < value_) {
            if (left_child_ != nullptr) {
                return left_child_->search(value);
            }
        } else {
            if (right_child_ != nullptr) {
                return right_child_->search(value);
            }
        }
        return nullptr;
    }

    Binary_Node* search_with_loop(const T& value)
    {
        auto node = this;
        while (node != nullptr) {
            if (node->value_ == value) {
                return node;
            }

            if (value < node->value_) {
                node = node->left_child_;
            } else {
                node = node->right_child_;
            }
        }

        return nullptr;
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

    void insert(T value)
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
};

int main()
{
    auto root = new Binary_Node<int>(1);

    for (const auto i : { 2, 3, 12, 8, 9, 10, 5, 6 }) {
        root->insert(i);
    }
    root->print();

    auto k = root->search(9);
    k->remove_self();
    root->print();

    k = root->search(8);
    k->remove_self();
    root->print();

    delete root;
    return 0;
}