#ifndef FIXED_ALLOCATOR_HPP

#define FIXED_ALLOCATOR_HPP

#include "chunk_block.hpp"

#include <cstddef>
#include <cassert>

template <size_t chunkSize>
class ChunkBlock;

template <size_t chunkSize>
class FixedAllocator {

public:

    static FixedAllocator<chunkSize>& getInstance();

    void* allocate();

    void deallocate(void* pointer);

    FixedAllocator& operator=(const FixedAllocator &other) = delete;

private:

    FixedAllocator();

    ChunkBlock<chunkSize> *head_;
    ChunkBlock<chunkSize> *tail_;
    char *allocationPointer_;

    static FixedAllocator<chunkSize> *instance_;

};

template <size_t chunkSize>
FixedAllocator<chunkSize>* FixedAllocator<chunkSize>::instance_ = nullptr;

template <size_t chunkSize>
FixedAllocator<chunkSize>& FixedAllocator<chunkSize>::getInstance() {
    if (!instance_) {
        instance_ = new FixedAllocator<chunkSize>();
    }
    return *instance_;
}

template <size_t chunkSize>
FixedAllocator<chunkSize>::FixedAllocator() {
    head_ = new ChunkBlock<chunkSize>();
    tail_ = head_;
    allocationPointer_ = head_->getPool();
}

template <size_t chunkSize>
void* FixedAllocator<chunkSize>::allocate() {
    if (!allocationPointer_) {
        ChunkBlock<chunkSize>::linkBlocks(tail_, new ChunkBlock<chunkSize>());
        tail_ = tail_->getNextBlock();
        allocationPointer_ = tail_->getPool();
    }

    void* result = static_cast<void*>(allocationPointer_);
    char *newAllocationPointer_ = *reinterpret_cast<char**>(allocationPointer_);
    allocationPointer_ = newAllocationPointer_;
    return result;
}

template <size_t chunkSize>
void FixedAllocator<chunkSize>::deallocate(void *pointer) {
    char **jumpPointer = reinterpret_cast<char**>(pointer);
    *jumpPointer = allocationPointer_;
    allocationPointer_ = reinterpret_cast<char*>(pointer);
}

#endif