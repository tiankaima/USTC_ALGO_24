#include <algorithm>
#include <iostream>

template <typename T>
struct LinkedList {
    T data;
    LinkedList* next;
};

template <typename T>
struct Queue {
    LinkedList<T>* head;
    LinkedList<T>* tail;
};

template <typename T>
void enqueue(Queue<T>* queue, T data)
{
    auto* node = new LinkedList<T>;
    node->data = data;
    node->next = nullptr;

    if (queue->head == nullptr) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
}

template <typename T>
LinkedList<T>* dequeue(Queue<T>* queue)
{
    if (queue->head == nullptr)
        return nullptr;

    T data = queue->head->data;
    LinkedList<T>* node = queue->head;
    queue->head = queue->head->next;

    if (queue->head == nullptr)
        queue->tail = nullptr;

    return node;
}

template <typename T>
struct TwithD {
    T data;
    int distance;
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N, M, S, T;
    std::cin >> N >> M >> S >> T;

    auto* nexts = new LinkedList<int>[N];
    for (int i = 0; i < N; i++)
        nexts[i].next = nullptr;

    for (int i = 0; i < M; i++) {
        int u, v;
        std::cin >> u >> v;
        auto* node = new LinkedList<int>;
        node->data = v;
        node->next = nexts[u].next;
        nexts[u].next = node;
    }

    // BFS:
    Queue<TwithD<int>> queue {
        nullptr, nullptr
    };
    enqueue(&queue, { S, 0 });

    bool* visited = new bool[N];
    std::fill(visited, visited + N, false);

    while (queue.head != nullptr) {
        LinkedList<TwithD<int>>* node = dequeue(&queue);
        int u = node->data.data;
        int d = node->data.distance;
        delete node;

        if (u == T) {
            std::cout << d << std::endl;

            delete[] visited;
            delete[] nexts;
            return 0;
        }

        if (visited[u])
            continue;

        visited[u] = true;

        for (LinkedList<int>* p = nexts[u].next; p != nullptr; p = p->next) {
            int v = p->data;
            if (!visited[v])
                enqueue(&queue, { v, d + 1 });
        }
    }

    std::cout << "-1" << std::endl;

    delete[] visited;
    delete[] nexts;
    return 0;
}