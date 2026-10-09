#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

class ChunkedList {
    struct Node {
        std::vector<long long> values; // Contiguous array inside each list node.
        Node* next = nullptr;
    };
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t block_size_;
    std::size_t size_ = 0;

    struct Cursor {
        Node* node;
        std::size_t offset = 0;
        long long& value() const { return node->values[offset]; }
        void advance() {
            ++offset;
            if (offset == node->values.size()) {
                node = node->next;
                offset = 0;
            }
        }
        void skip(std::size_t count) {
            for (std::size_t i = 0; i < count; ++i) advance();
        }
    };
public:
    explicit ChunkedList(std::size_t block_size) : block_size_(block_size) {}
    ChunkedList(const ChunkedList&) = delete;
    ChunkedList& operator=(const ChunkedList&) = delete;
    ~ChunkedList() {
        while (head_) {
            Node* next = head_->next;
            delete head_;
            head_ = next;
        }
    }
    void push_back(long long value) {
        if (!tail_ || tail_->values.size() == block_size_) {
            Node* node = new Node;
            node->values.reserve(block_size_);
            if (tail_) tail_->next = node;
            else head_ = node;
            tail_ = node;
        }
        tail_->values.push_back(value);
        ++size_;
    }
    void merge_sort() {
        std::vector<long long> buffer(size_); // Exactly one extra element array.
        for (std::size_t width = 1; width < size_; width *= 2) {
            Cursor segment{head_};
            std::size_t out = 0;
            for (std::size_t start = 0; start < size_;) {
                const std::size_t left_count = std::min(width, size_ - start);
                const std::size_t right_count = std::min(width, size_ - start - left_count);
                Cursor left = segment, right = segment;
                right.skip(left_count);
                std::size_t i = 0, j = 0;
                while (i < left_count && j < right_count) {
                    if (left.value() <= right.value()) {
                        buffer[out++] = left.value();
                        left.advance(); ++i;
                    } else {
                        buffer[out++] = right.value();
                        right.advance(); ++j;
                    }
                }
                while (i < left_count) {
                    buffer[out++] = left.value();
                    left.advance(); ++i;
                }
                while (j < right_count) {
                    buffer[out++] = right.value();
                    right.advance(); ++j;
                }
                const std::size_t consumed = left_count + right_count;
                segment.skip(consumed);
                start += consumed;
            }
            Cursor destination{head_};
            for (long long value : buffer) {
                destination.value() = value;
                destination.advance();
            }
        }
    }
    void print() const {
        bool first = true;
        for (Node* node = head_; node; node = node->next) {
            for (long long value : node->values) {
                if (!first) std::cout << ' ';
                first = false;
                std::cout << value;
            }
        }
        std::cout << '\n';
    }
};

int main() {
    std::size_t n, block_size;
    if (!(std::cin >> n >> block_size) || block_size == 0) return 1;
    ChunkedList list(block_size);
    for (std::size_t i = 0; i < n; ++i) {
        long long value;
        if (!(std::cin >> value)) return 1;
        list.push_back(value);
    }
    list.merge_sort();
    list.print();
}
