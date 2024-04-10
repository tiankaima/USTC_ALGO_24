#include <stdio.h>
#include <stdlib.h>

// queue item:
struct queue_item {
    int value;
    struct queue_item* next;
};

int main()
{
    int n;
    scanf("%d", &n);

    int op;
    int tmp;

    struct queue_item* head = NULL;

    for (int i = 0; i < n; i++) {
        scanf("%d", &op);
        switch (op) {
        case 1:
            scanf("%d", &tmp);
            if (head == NULL) {
                head = (struct queue_item*)malloc(sizeof(struct queue_item));
                head->value = tmp;
                head->next = NULL;
                break;
            }

            struct queue_item* new_item = (struct queue_item*)malloc(sizeof(struct queue_item));
            struct queue_item* tmp_head = head;
            while (tmp_head->next != NULL) {
                tmp_head = tmp_head->next;
            }
            tmp_head->next = new_item;
            new_item->value = tmp;
            new_item->next = NULL;
            break;
        case 2:
            if (head == NULL) {
                printf("ERR_CANNOT_POP\n");
            } else {
                head = head->next;
            }
            break;
        case 3:
            if (head == NULL) {
                printf("ERR_CANNOT_QUERY\n");
            } else {
                printf("%d\n", head->value);
            }
            break;
        case 4:
            if (head == NULL) {
                printf("0\n");
                ;
            } else {
                int size = 1;
                struct queue_item* tmp_head = head;
                while (tmp_head->next != NULL) {
                    size++;
                    tmp_head = tmp_head->next;
                }
                printf("%d\n", size);
            }
            break;
        default:
            break;
        }
    }
}