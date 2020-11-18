#ifndef MAP_ALLOCATOR_H
#define MAP_ALLOCATOR_H

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <memory>

template<typename T, int N>
struct map_allocator {
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*; 
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = map_allocator<U, N>;
    };

    map_allocator() {
        mem_p = nullptr;
        next_mem = 0;
        container_size = N;
        chunk_size = 0;
        count = 0;
    }
    ~map_allocator() {
        for(const auto& addr: chunck_addrs)
            std::free(addr);
    }

    template<typename U, int M> 
    map_allocator(const map_allocator<U, M>&) {}

    void* mem_p;
    int next_mem;
    int count;
    int chunk_size;
    int container_size;
    std::stack<T*> free_addr;
    std::vector<void*> chunck_addrs;

    T *allocate(std::size_t n) {
        void* addr = 0;
        if (mem_p == nullptr)
        {
            container_size = N;
            mem_p = std::malloc(n * N * sizeof(T));
            if (!mem_p)
                throw std::bad_alloc();
            addr = mem_p;
            next_mem = 1;
            chunck_addrs.push_back(mem_p);
            chunk_size = N;
        }
        else
        {
            if (count==container_size)
            {
                chunk_size = container_size;
                container_size *= 2;
                auto new_mem_p = std::malloc(container_size * sizeof(T));
                if (!new_mem_p)
                    throw std::bad_alloc();
                mem_p = new_mem_p;
                chunck_addrs.push_back(mem_p);
                next_mem = 0;
            }
            if (next_mem!=chunk_size)
            {
                addr = reinterpret_cast<T *>(mem_p) + next_mem;
                next_mem++;
            }
            else 
            {
                addr = free_addr.top();
                free_addr.pop();
            }   
        }
        count++;
        return reinterpret_cast<T *>(addr);
    }

    void deallocate(T *p, std::size_t n) {
        (void)n;
        free_addr.push(p);
        count--;
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(T *p) {
        p->~T();
    }
};

#endif //MAP_ALLOCATOR_H