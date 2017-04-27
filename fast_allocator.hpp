#ifndef FAST_ALLOCATOR_HPP

#define FAST_ALLOCATOR_HPP

#include "fixed_allocator.hpp"

#include <cstdio>
#include <cstddef>
#include <cassert>
#include <utility>
#include <set>

template <typename Type>
class FastAllocator {

public:

    typedef Type value_type;

    FastAllocator();

    template <typename OtherType>
    FastAllocator(const FastAllocator<OtherType> &other) {}

    template <typename OtherType>
    FastAllocator(FastAllocator<OtherType> &&other) {}

    Type* allocate(size_t amount);

    void deallocate(Type *pointer, size_t amount);

    template <typename Constructible, typename... Arguments>
    void construct(Constructible *pointer, Arguments&&... arguments);

    template <typename Destructible>
    void destroy(Destructible *pointer);

    bool operator==(const FastAllocator<Type> &other) {
        return true;
    }

    bool operator!=(const FastAllocator<Type> &other) {
        return false;
    }

private:

    FixedAllocator<sizeof(Type)> *allocator_;

};

template <typename Type>
FastAllocator<Type>::FastAllocator() {
    allocator_ = &FixedAllocator<sizeof(Type)>::getInstance();
}

template <typename Type>
Type* FastAllocator<Type>::allocate(size_t amount) {
    assert(amount);

    if (amount == 1) {
        return static_cast<Type*>(allocator_->allocate());
    }

    return new Type[amount];
}

template <typename Type>
void FastAllocator<Type>::deallocate(Type *pointer, size_t amount) {
    assert(amount);

    if (amount == 1) {
        allocator_->deallocate(pointer);
    } else {
        delete[] pointer;
    }
}

template <typename Type>
template <typename Constructible, typename... Arguments>
void FastAllocator<Type>::construct(Constructible *pointer, Arguments&&... arguments) {
    new (pointer) Constructible(std::forward<Arguments>(arguments)...);
}

template <typename Type>
template <typename Destructible>
void FastAllocator<Type>::destroy(Destructible *pointer) {
    pointer->~Destructible();
}

#endif