#ifndef CHUNK_POOL_HPP

#define CHUNK_POOL_HPP

#include <cstddef>
#include <cassert>

template <size_t chunkSize>
class ChunkBlock {

public:

    const size_t MINIMUM_SIZE = 4096;

    static void linkBlocks(ChunkBlock<chunkSize> *firstBlock,
                           ChunkBlock<chunkSize> *lastBlock);

    ChunkBlock();

    ~ChunkBlock();

    ChunkBlock<chunkSize>* getNextBlock() const;

    ChunkBlock<chunkSize>* getPreviousBlock() const;

    char* getPool();

private:

    ChunkBlock<chunkSize> *nextBlock_;
    ChunkBlock<chunkSize> *previousBlock_;
    char *pool_;
    size_t realSize_;
    size_t chunksCount_;

};

template <size_t chunkSize>
void ChunkBlock<chunkSize>::linkBlocks(ChunkBlock<chunkSize> *firstBlock,
                                       ChunkBlock<chunkSize> *lastBlock) {
    if (firstBlock)
        firstBlock->nextBlock_ = lastBlock;
    if (lastBlock)
        lastBlock->previousBlock_ = firstBlock;
}

template <size_t chunkSize>
ChunkBlock<chunkSize>::ChunkBlock() {
    nextBlock_ = nullptr;
    previousBlock_ = nullptr;

    realSize_ = chunkSize > sizeof(char*) ? chunkSize : sizeof(char);
    chunksCount_ = (MINIMUM_SIZE + realSize_ - 1) / realSize_;
    pool_ = new char[realSize_ * chunksCount_];

    char **jumpPointer;
    for (size_t index = 0; index < chunksCount_ - 1; ++index) {
        jumpPointer = reinterpret_cast<char**>(pool_ + index * realSize_);
        *jumpPointer = pool_ + (index + 1) * realSize_;
    }

    jumpPointer = reinterpret_cast<char**>(pool_ + (chunksCount_ - 1) * realSize_);
    *jumpPointer = nullptr;
}

template <size_t chunkSize>
ChunkBlock<chunkSize>::~ChunkBlock() {
    delete[] pool_;
}

template <size_t chunkSize>
ChunkBlock<chunkSize>* ChunkBlock<chunkSize>::getNextBlock() const {
    return nextBlock_;
}

template <size_t chunkSize>
ChunkBlock<chunkSize>* ChunkBlock<chunkSize>::getPreviousBlock() const {
    return previousBlock_;
}

template <size_t chunkSize>
char* ChunkBlock<chunkSize>::getPool() {
    return pool_;
}

#endif