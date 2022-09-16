#include "MemoryManager.hpp"
#include <cstdlib>

using namespace OctoPotato;

namespace OctoPotato {
    static const uint32_t kBlockSizes[] = {
        // 4-increments
        4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
        52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,

        // 32-increments
        128, 160, 192, 224, 256, 288, 320, 352, 384,
        416, 448, 480, 512, 544, 576, 608, 640,

        // 64-increments
        704, 768, 832, 896, 960, 1024
    };

    static const uint32_t kPageSize = 8192;
    static const uint32_t kAlignment = 4;

    static const uint32_t kNumBlockSizes = sizeof(kBlockSizes) / sizeof(*kBlockSizes);

    static const uint32_t kMaxBlockSize = kBlockSizes[kNumBlockSizes - 1];
}

size_t*     MemoryManager::m_pBlockSizeLookup;
Allocator*  MemoryManager::m_pAllocators;

int MemoryManager::initialize() {
    static bool s_bInitialized = false;
    if (!s_bInitialized) {
        m_pBlockSizeLookup = new size_t[kMaxBlockSize + 1];
        size_t j = 0;
        for (size_t i = 0; i <= kMaxBlockSize; ++i) {
            if (i > kBlockSizes[j]) ++j;
            m_pBlockSizeLookup[i] = j;
        }

        m_pAllocators = new Allocator[kNumBlockSizes];
        for (size_t i = 0; i < kNumBlockSizes; ++i) {
            m_pAllocators[i].reset(kBlockSizes[i], kPageSize, kAlignment);
        }

        s_bInitialized = true;
    }

    return 0;
}

void MemoryManager::finalize() {
    delete[] m_pAllocators;
    delete[] m_pBlockSizeLookup;
}

void MemoryManager::tick() {}

Allocator* MemoryManager::lookUpAllocator(size_t size) {
    if (size <= kMaxBlockSize) {
        return m_pAllocators + m_pBlockSizeLookup[size];
    }
    else {
        return nullptr;
    }
}

void* MemoryManager::allocate(size_t size) {
    Allocator* pAlloc = lookUpAllocator(size);
    if (pAlloc) {
        return pAlloc->allocate();
    }
    else {
        return std::malloc(size);
    }
}

void MemoryManager::free(void* p, size_t size) {
    Allocator* pAlloc = lookUpAllocator(size);
    if (pAlloc)
        pAlloc->free(p);
    else
        std::free(p);
}