#include <iostream>

struct Node {
    long long value;
    Node* next = nullptr;
};

// Cut after at most width nodes; return the next run.
Node* split(Node* head, std::size_t width) {
    if (!head) return nullptr;
    while (--width && head->next) head = head->next;
    Node* next = head->next;
    head->next = nullptr;
    return next;
}

// Merge two null-terminated runs by relinking existing nodes.
// The dummy lives on the stack; no new list node is allocated.
Node* merge(Node* left, Node* right, Node*& tail) {
    Node dummy{0};
    tail = &dummy;
    while (left && right) {
        Node*& source = (left->value <= right->value) ? left : right;
        tail->next = source;
        source = source->next;
        tail = tail->next;
    }
    tail->next = left ? left : right;
    while (tail->next) tail = tail->next;
    return dummy.next;
}

Node* merge_sort(Node* head, std::size_t n) {
    for (std::size_t width = 1; width < n; width *= 2) {
        Node* current = head;
        Node* result = nullptr;
        Node* result_tail = nullptr;
        while (current) {
            Node* left = current;
            Node* right = split(left, width);
            current = split(right, width);
            Node* merged_tail = nullptr;
            Node* merged = merge(left, right, merged_tail);
            if (result_tail) result_tail->next = merged;
            else result = merged;
            result_tail = merged_tail;
        }
        head = result;
    }
    return head;
}

int main() {
    std::size_t n;
    if (!(std::cin >> n)) return 1;
    Node* head = nullptr;
    Node** tail_link = &head;
    for (std::size_t i = 0; i < n; ++i) {
        long long value;
        if (!(std::cin >> value)) return 1;
        *tail_link = new Node{value};
        tail_link = &(*tail_link)->next;
    }
    head = merge_sort(head, n);
    bool first = true;
    while (head) {
        if (!first) std::cout << ' ';
        first = false;
        std::cout << head->value;
        Node* next = head->next;
        delete head;
        head = next;
    }
    std::cout << '\n';
}
