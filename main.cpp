#include "fast_allocator.hpp"
#include "list.hpp"
#include <list>
#include <cstdio>
#include <ctime>

enum EOps {
    PUSH_BACK,
    POP_BACK,
    INSERT_AFTER,
    INSERT_BEFORE
};

int main() {
    List<int> l;
    std::list<int> l2;

    int opcode;
    int value;
    for (int i = 0; i < 1000; ++i) {
        opcode = rand() % 4;
        value = rand();
        l.push_back(value);
        l2.push_back(value);
    }

    typename List<int>::Node *node = l.begin();
    auto it = l2.begin();
    while (node) {
        assert(node->value == *it);
        node = node->nextNode;
        ++it;
    }


    return 0;
}
