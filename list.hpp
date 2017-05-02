#ifndef LIST_HPP

#define LIST_HPP

#include <cassert>
#include <memory>

template <typename Type>
struct NodeType {
    Type value;
    NodeType<Type> *nextNode;
    NodeType<Type> *previousNode;

    NodeType(const Type &nodeValue) {
        value = nodeValue;
        nextNode = previousNode = nullptr;
    }

    NodeType(Type &&nodeValue) {
        value = std::move(nodeValue);
        nextNode = previousNode = nullptr;
    }
};

template <typename Type, typename Allocator = std::allocator<Type>>
class List {

public:

    typedef NodeType<Type> Node;

    typedef typename std::allocator_traits<Allocator>::template rebind_alloc<Node> AllocatorType;

    explicit List(const Allocator &allocator = Allocator());

    List(size_t count, const Type& value = Type(), const Allocator &allocator = Allocator());

    size_t size() const;

    void push_back(const Type &value);

    void push_back(Type &&value);

    void pop_back();

    void push_front(const Type &value);

    void push_front(Type &&value);

    void pop_front();

    void insert_before(Node *node, const Type &value);

    void insert_before(Node *node, Type &&value);

    void insert_after(Node *node, const Type &value);

    void insert_after(Node *node, Type &&value);

    void erase(Node *node);

    Node* begin();

private:

    Node* createNode(const Type &value);

    Node* createNode(Type &&value);

    void destroyNode(Node *node);

    Node* linkNodes(Node *firstNode, Node *lastNode,
                           bool returnFirst = true);

    void fixEndPointers(Node* &brokenPointer, Node* &correctPointer);

    Node *head_;
    Node *tail_;

    size_t size_;

    AllocatorType allocator_;

};

template <typename Type, typename Allocator>
List<Type, Allocator>::List(const Allocator &allocator) {
    allocator_ = AllocatorType(allocator);
    head_ = tail_ = nullptr;
    size_ = 0;
}

template <typename Type, typename Allocator>
List<Type, Allocator>::List(size_t count, const Type& value, const Allocator &allocator) : List(allocator) {
    while (count--) {
        push_back(value);
    }
} 

template <typename Type, typename Allocator>
size_t List<Type, Allocator>::size() const {
    return size_;
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::push_back(const Type &value) {
    ++size_;
    tail_ = linkNodes(tail_, createNode(value), false);
    fixEndPointers(head_, tail_);
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::push_back(Type &&value) {
    ++size_;
    tail_ = List<Type, Allocator>::linkNodes(tail_,
                                             createNode(std::move(value)), false);
    fixEndPointers(head_, tail_);
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::pop_back() {
    assert(size_);
    --size_;

    Node *formerTail = tail_;
    tail_ = List<Type, Allocator>::linkNodes(tail_->previousNode, nullptr);
    destroyNode(formerTail);

    fixEndPointers(head_, tail_);
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::push_front(const Type &value) {
    ++size_;
    head_ = List<Type, Allocator>::linkNodes(createNode(value), head_);
    fixEndPointers(tail_, head_);
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::push_front(Type &&value) {
    ++size_;
    tail_ = List<Type, Allocator>::linkNodes(createNode(std::move(value)),
                                             head_);
    fixEndPointers(tail_, head_);
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::pop_front() {
    assert(size_);
    --size_;

    Node *formerHead = head_;
    head_ = List<Type, Allocator>::linkNodes(nullptr, head_->nextNode, false);
    destroyNode(formerHead);

    fixEndPointers(tail_, head_);
}

template <typename Type, typename Allocator>
typename List<Type, Allocator>::Node* List<Type, Allocator>::createNode(const Type &value) {
    Node *newNode = allocator_.allocate(1);
    new (newNode) Type(value);
    return newNode;
}


template <typename Type, typename Allocator>
typename List<Type, Allocator>::Node* List<Type, Allocator>::createNode(Type &&value) {
    Node *newNode = allocator_.allocate(1);
    new (newNode) Type(std::move(value));
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::destroyNode(Node *node) {
    node->~Node();
    allocator_.deallocate(node, 1);
}

template <typename Type, typename Allocator>
typename List<Type, Allocator>::Node* List<Type, Allocator>::linkNodes(Node *firstNode,
                                                                       Node *lastNode,
                                                                       bool returnFirst) {
    if (firstNode) {
        firstNode->nextNode = lastNode;
    }

    if (lastNode) {
        lastNode->previousNode = firstNode;
    }

    return returnFirst ? firstNode : lastNode;
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::fixEndPointers(Node* &brokenPointer,
                                           Node* &correctPointer) {
    if (!correctPointer) {
        brokenPointer = nullptr;
    } else if (!brokenPointer) {
        brokenPointer = correctPointer;
    }
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::insert_after(Node *node, const Type &value) {
    if (node == tail_) {
        push_back(value);
    } else {
        List<Type, Allocator>::linkNodes(node,
                                         List<Type, Allocator>::linkNodes(createNode(value),
                                                                          node->nextNode));
    }
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::insert_after(Node *node, Type &&value) {
    if (node == tail_) {
        push_back(std::move(value));
    } else {
        List<Type, Allocator>::linkNodes(node,
                                         List<Type, Allocator>::linkNodes(
                                                 createNode(std::move(value)),
                                                 node->nextNode));
    }
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::insert_before(Node *node, const Type &value) {
    if (node == head_) {
        push_front(value);
    } else {
        List<Type, Allocator>::linkNodes(List<Type, Allocator>::linkNodes(
                node->previousNode,
                createNode(value)), node);
    }
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::insert_before(Node *node, Type &&value) {
    if (node == head_) {
        push_front(std::move(value));
    } else {
        List<Type, Allocator>::linkNodes(List<Type, Allocator>::linkNodes(
                node->previousNode,
                createNode(std::move(value))), node);
    }
}

template <typename Type, typename Allocator>
void List<Type, Allocator>::erase(Node *node) {
    if (node == head_) {
        pop_front();
    } else if (node == tail_) {
        pop_back();
    } else {
        List<Type, Allocator>::linkNodes(node->previousNode, node->nextNode);
        destroyNode(node);
    }
}

template <typename Type, typename Allocator>
typename List<Type, Allocator>::Node* List<Type, Allocator>::begin() {
    return head_;
}

#endif
