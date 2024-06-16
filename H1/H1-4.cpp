#include <iostream>

template <typename T>
struct LinkListItem {
    T data;
    LinkListItem* next;
};

template <typename T>
void plug(LinkListItem<T>* head, T value, T after_which)
{
    LinkListItem<T>* new_item = new LinkListItem<T>;
    new_item->data = value;
    LinkListItem<T>* current = head;
    while (current->data != after_which) {
        current = current->next;
    }
    new_item->next = current->next;
    current->next = new_item;
}

template <typename T>
T find_after(LinkListItem<T>* head, T value)
{
    LinkListItem<T>* current = head;
    while (current->data != value) {
        current = current->next;
    }
    if (current->next == nullptr) {
        return 0;
    }
    return current->next->data;
}

template <typename T>
void delete_after(LinkListItem<T>* head, T value)
{
    LinkListItem<T>* current = head;
    while (current->data != value) {
        current = current->next;
    }
    if (current->next == nullptr) {
        return;
    }
    LinkListItem<T>* temp = current->next;
    current->next = current->next->next;
    delete temp;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int total;
    std::cin >> total;
    LinkListItem<int> head;
    head.data = 1;
    head.next = nullptr;
    for (int i = 0; i < total; i++) {
        static int op, tmp1, tmp2;
        std::cin >> op;
        switch (op) {
        case 1:
            std::cin >> tmp1 >> tmp2;
            plug(&head, tmp2, tmp1);
            break;
        case 2:
            std::cin >> tmp1;
            std::cout << find_after(&head, tmp1) << std::endl;
            break;
        case 3:
            std::cin >> tmp1;
            delete_after(&head, tmp1);
        default:
            break;
        }
    }
}