#pragma once
#include "IRuntimeModule.hpp"
#include "Allocator.hpp"
#include <new>

namespace OctoPotato {
    class MemoryManager : implements IRuntimeModule {
    public:
        template<typename T, typename... Arguments>
        T* New(Arguments... parameters) {
            return new (allocate(sizeof(T))) T(parameters...);
        }

        template<typename T>
        void Delete(T *p) {
            reinterpret_cast<T*>(p)->~T();
            free(p, sizeof(T));
        }

        virtual ~MemoryManager() {}

        virtual int initialize();
        virtual void finalize();
        virtual void tick();

        void * allocate(size_t size);
        void free(void *p, size_t size);

    private:
        static size_t       *m_pBlockSizeLookup;
        static Allocator    *m_pAllocators;

        static Allocator * lookUpAllocator(size_t size);
    };
}