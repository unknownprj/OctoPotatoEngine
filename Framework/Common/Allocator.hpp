#pragma once

#include <cstddef>
#include <cstdint>

namespace OctoPotato {
    struct BlockHeader {
        BlockHeader* next;
    };

    struct PageHeader {
        PageHeader* next;

        BlockHeader* blocks() { return reinterpret_cast<BlockHeader*>(this + 1); }
    };

    class Allocator {
    public:
        static const uint8_t PATTERN_ALIGN = 0xFC;
        static const uint8_t PATTERN_ALLOC = 0xFD;
        static const uint8_t PATTERN_FREE = 0xFE;

        Allocator();
        Allocator(size_t dataSize, size_t pageSize, size_t alignment);

        ~Allocator();

        void reset(size_t dataSize, size_t pageSize, size_t alignment);

        void* allocate();

        void free(void* p);

        void freeAll();

    private:
#if defined(_DEBUG)
        void fillFreePage(PageHeader* p);

        void fillFreeBlock(BlockHeader* p);

        void fillAllocatedBlock(BlockHeader* p);
#endif
        BlockHeader* nextBlock(BlockHeader* p);

        PageHeader* m_pageList;

        BlockHeader* m_freeList;

        size_t m_dataSize;
        size_t m_pageSize;
        size_t m_alignmentSize;
        size_t m_blockSize;

        uint32_t m_blocksPerPage;
        uint32_t m_numPages;
        uint32_t m_numBlocks;
        uint32_t m_numFreeBlocks;

        Allocator(const Allocator&);
        Allocator& operator=(const Allocator&);
    };
}