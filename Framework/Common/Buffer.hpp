#pragma once

#include <cstddef>
#include "MemoryManager.hpp"

namespace OctoPotato {
    extern MemoryManager* g_pMemManager;
    class Buffer {
    public:
        Buffer() : m_pData(nullptr), m_size(0), m_alignment(alignof(uint32_t)) {}
        Buffer(size_t size, size_t alignment = 4) : m_size(size), m_alignment(alignment) {
            m_pData = reinterpret_cast<uint8_t*>(g_pMemManager->allocate(size));
        }
        ~Buffer() {
            if (m_pData)
                g_pMemManager->free(m_pData, m_size);
            m_pData = nullptr;
        }

        Buffer(const Buffer& rhs) {
            m_pData = reinterpret_cast<uint8_t*>(g_pMemManager->allocate(rhs.m_size));
            memcpy(m_pData, rhs.m_pData, rhs.m_size);
            m_size = rhs.m_size;
            m_alignment = rhs.m_alignment;
        }

        Buffer(Buffer&& rhs) {
            m_pData = rhs.m_pData;
            m_size = rhs.m_size;
            m_alignment = rhs.m_alignment;
            rhs.m_pData = nullptr;
            rhs.m_size = 0;
            rhs.m_alignment = 4;
        }

        Buffer& operator = (const Buffer& rhs) {
            if (this == &rhs)
                return *this;
            if (m_size >= rhs.m_size && m_alignment == rhs.m_alignment) {
                memcpy(m_pData, rhs.m_pData, rhs.m_size);
            }
            else {
                if (m_pData) g_pMemManager->free(m_pData, m_size);
                m_pData = reinterpret_cast<uint8_t*>(g_pMemManager->allocate(rhs.m_size));
                memcpy(m_pData, rhs.m_pData, rhs.m_size);
                m_size = rhs.m_size;
                m_alignment = rhs.m_alignment;
            }
            return *this;
        }

        Buffer& operator = (Buffer&& rhs) {
            if (this == &rhs)
                return *this;
            if (m_pData) g_pMemManager->free(m_pData, m_size);
            m_pData = rhs.m_pData;
            m_size = rhs.m_size;
            m_alignment = rhs.m_alignment;
            rhs.m_pData = nullptr;
            rhs.m_size = 0;
            rhs.m_alignment = 4;
            return *this;
        }

        uint8_t* data() const { return m_pData; }

        size_t size() const { return m_size; }
    private:
        uint8_t* m_pData;
        size_t m_size;
        size_t m_alignment;
    };
}