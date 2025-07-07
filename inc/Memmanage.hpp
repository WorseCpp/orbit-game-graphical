#ifndef MEMMANAGE_VERTS_HPP
#define MemManage

#include <vector>
#include <stack>
#include <stdexcept>

class BlockAllocator {
public:
    BlockAllocator(size_t totalIndices, size_t blockSize)
        : totalIndices_(totalIndices), blockSize_(blockSize)
    {
        if (blockSize == 0 || totalIndices == 0 || totalIndices % blockSize != 0)
            throw std::invalid_argument("Invalid block size or total indices");
        numBlocks_ = totalIndices_ / blockSize_;
        freeBlocks_.reserve(numBlocks_);
        for (size_t i = 0; i < numBlocks_; ++i) {
            freeBlocks_.push_back(i);
        }
    }

    // Allocates a block and returns the starting index, or throws if none available
    size_t allocate() {
        if (freeBlocks_.empty())
            throw std::out_of_range("No free blocks available");
        size_t blockIdx = freeBlocks_.back();
        freeBlocks_.pop_back();
        return blockIdx * blockSize_;
    }

    // Deallocates a block given its starting index
    void deallocate(size_t startIndex) {
        if (startIndex % blockSize_ != 0)
            throw std::invalid_argument("Invalid block start index");
        size_t blockIdx = startIndex / blockSize_;
        if (blockIdx >= numBlocks_ || blockIdx < 0)
            throw std::out_of_range("Block index out of range");
        freeBlocks_.push_back(blockIdx);
    }

    size_t blockSize() const { return blockSize_; }
    size_t totalBlocks() const { return numBlocks_; }
    size_t freeBlockCount() const { return freeBlocks_.size(); }
    

    void reset() {
        freeBlocks_.clear();
        for (size_t i = 0; i < numBlocks_; ++i) {
            freeBlocks_.push_back(i);
        }
    }

private:
    size_t totalIndices_;
    size_t blockSize_;
    size_t numBlocks_;
    std::vector<size_t> freeBlocks_;
};

#endif