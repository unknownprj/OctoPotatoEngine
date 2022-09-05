#include "Allocator.hpp"
#include <cassert>
#include <cmath>
#include <cstring>

#ifndef ALIGN
#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) -1))
#endif

using namespace OctoPotato;

Allocator::Allocator(size_t dataSize, size_t pageSize, size_t alignment) : m_pageList(nullptr), m_freeList(nullptr) {
    reset(dataSize, pageSize, alignment);
}

Allocator::~Allocator() {
    freeAll();
}

void Allocator::reset(size_t dataSize, size_t pageSize, size_t alignment) {
    freeAll();

    m_dataSize = dataSize;
    m_pageSize = pageSize;

    size_t maxHeaderData = __max(sizeof(BlockHeader), m_dataSize);

#if defined(_DEBUG)
    assert(alignment > 0 && (alignment & (alignment - 1)) == 0);
#endif
    m_blockSize = ALIGN(maxHeaderData, alignment);
    m_alignmentSize = m_blockSize - maxHeaderData;
    m_blocksPerPage = (m_pageSize - sizeof(PageHeader)) / m_blockSize;
}

void * Allocator::allocate() {
    if (!m_freeList) {
        PageHeader *newPage = reinterpret_cast<PageHeader *>(new uint8_t[m_pageSize]);
        ++m_numPages;
        m_numBlocks     += m_blocksPerPage;
        m_numFreeBlocks += m_blocksPerPage;
#if defined(_DEBUG)
        fillFreePage(newPage);
#endif
        newPage->next = m_pageList;
        m_pageList = newPage;

        BlockHeader *currBlock = newPage->blocks();
        for(uint32_t i = 0; i < m_blocksPerPage - 1; ++i) {
            currBlock->next = nextBlock(currBlock);
            currBlock = nextBlock(currBlock);
        }
        currBlock->next = nullptr;
        m_freeList = newPage->blocks();
    }

    BlockHeader *freeBlock = m_freeList;
    m_freeList = m_freeList->next;
    --m_numFreeBlocks;
#if defined(_DEBUG)
    fillAllocatedBlock(freeBlock);
#endif
    return reinterpret_cast<void*>(freeBlock);
}

void Allocator::free(void *p) {
    BlockHeader *block = reinterpret_cast<BlockHeader *>(p);
#if defined(_DEBUG)
    fillFreeBlock(block);
#endif
    block->next = m_freeList;
    m_freeList = block;
    ++m_numFreeBlocks;
}

void Allocator::freeAll() {
    PageHeader *pageWalker = m_pageList;
    while(pageWalker) {
        PageHeader *currPage = pageWalker;
        pageWalker = pageWalker->next;

        delete[] reinterpret_cast<uint8_t *>(currPage);
    }
    m_pageList = nullptr;
    m_freeList = nullptr;

    m_numBlocks     = 0;
    m_numFreeBlocks = 0;
    m_numPages      = 0;
}
#if defined(_DEBUG)
void Allocator::fillFreePage(PageHeader *p) {
    p->next = nullptr;

    BlockHeader *currBlock = p->blocks();
    for(uint32_t i = 0; i < m_blocksPerPage; ++i) {
        fillFreeBlock(currBlock);
        currBlock = nextBlock(currBlock);
    }
}

void Allocator::fillFreeBlock(BlockHeader *p) {
    std::memset(p, PATTERN_FREE, m_blockSize - m_alignmentSize);
    std::memset(reinterpret_cast<uint8_t *>(p) + m_blockSize - m_alignmentSize, PATTERN_ALIGN, m_alignmentSize);
}

void Allocator::fillAllocatedBlock(BlockHeader *p) {
    std::memset(p, PATTERN_ALLOC, m_blockSize - m_alignmentSize);
    std::memset(reinterpret_cast<uint8_t *>(p) + m_blockSize - m_alignmentSize, PATTERN_ALIGN, m_alignmentSize);
}
#endif
BlockHeader * Allocator::nextBlock(BlockHeader *p) {
    return reinterpret_cast<BlockHeader *>(reinterpret_cast<uint8_t *>(p) + m_blockSize);
}