#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#define PRINT_BH_WITH_NODE 1

struct RB_Node {
    int value_;
    bool red_;

    struct RB_Node* left_child_;
    struct RB_Node* right_child_;
    struct RB_Node* parent_;
};

int check_and_return_black_height(struct RB_Node* node);

void print(struct RB_Node* node, int depth) // NOLINT(*-no-recursion)
{
    if (node == NULL)
        return;

    print(node->right_child_, depth + 1);
    for (int i = 0; i < depth; i++)
        printf("\t");

    if (node->red_)
        printf("\033[31m");
    else
        printf("\033[0m");

#ifdef PRINT_BH_WITH_NODE
    printf("%d[%d]\n", node->value_, check_and_return_black_height(node));
#else
    printf("%d\n", node->value_);
#endif

    print(node->left_child_, depth + 1);
}

struct RB_Node* construct_BST_preorder(int* values, bool* reds, int i, int j) // NOLINT(*-no-recursion)
{

    if (i >= j)
        return NULL;

    struct RB_Node* node = (struct RB_Node*)malloc(sizeof(struct RB_Node));
    node->value_ = values[i];
    node->red_ = reds[i];
    node->left_child_ = NULL;
    node->right_child_ = NULL;
    node->parent_ = NULL;

    int k = i + 1;
    while (k < j && values[k] < values[i])
        k++;

    node->left_child_ = construct_BST_preorder(values, reds, i + 1, k);
    node->right_child_ = construct_BST_preorder(values, reds, k, j);

    if (node->left_child_ != NULL)
        node->left_child_->parent_ = node;
    if (node->right_child_ != NULL)
        node->right_child_->parent_ = node;

    return node;
}

int check_and_return_black_height(struct RB_Node* node) // NOLINT(*-no-recursion)
{
    if (node == NULL) {
        return 1;
    }

    int left_bh = check_and_return_black_height(node->left_child_);
    int right_bh = check_and_return_black_height(node->right_child_);

    if (left_bh == -1 || right_bh == -1 || left_bh != right_bh)
        return -1;

    if (node->red_) {
        if ((node->left_child_ != NULL && node->left_child_->red_) || (node->right_child_ != NULL && node->right_child_->red_))
            return -1;
        return left_bh;
    } else {
        return left_bh + 1;
    }
}

void free_tree(struct RB_Node* node) // NOLINT(*-no-recursion)
{
    if (node == NULL)
        return;

    free_tree(node->left_child_);
    free_tree(node->right_child_);
    free(node);
}

int main()
{
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        int size;
        scanf("%d", &size);

        int* values = (int*)malloc(size * sizeof(int));
        bool* reds = (bool*)malloc(size * sizeof(bool));

        for (int j = 0; j < size; j++) {
            int tmp;
            scanf("%d", &tmp);
            values[j] = (tmp > 0) ? tmp : -tmp;
            reds[j] = (tmp > 0) ? false : true;
        }

        struct RB_Node* root = construct_BST_preorder(values, reds, 0, size);
        //        print(root, 0);
        if (check_and_return_black_height(root) != -1)
            printf("Y\n");
        else
            printf("N\n");

        free_tree(root);
    }

    return 0;
}