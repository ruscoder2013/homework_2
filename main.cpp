#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <memory>
#include <cstring>

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
            mem_p = std::malloc(N * sizeof(T));
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
        free_addr.push(p);
        count--;
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        p->~T();
    }
};

template<class T, class Alloc = std::allocator<T>>
class Buffer {
public:
    Buffer() {
    }
    void emplace_back(T elem) {
        T* a = alloc.allocate(1);
        alloc.construct(a, elem);
        v.emplace_back(a);  
    }
    void pop_back() {
        auto el = v.back();
        alloc.destroy(el);
        alloc.deallocate(el, 1);
        v.pop_back();
    }

    typename std::vector<T*>::iterator begin() noexcept
    {
        return v.begin();
    }

    typename std::vector<T*>::iterator end() noexcept
    {
        return v.end();
    }

    void showAll() {
        for(auto el : v)
        {
            std::cout << *el << std::endl;
        }
    }
private:
    std::vector<T*> v;
    Alloc alloc;
};

constexpr int fact(int n) {
    int res = 1;
    for (int i = 1; i <= n; i++) {
	    res = res * i;
    }
    return res;
}

int main(int, char *[]) {
    auto m1 = std::map<int, int>{};
    for(int i = 0; i < 10; i++)
        m1[i] = fact(i);  

    auto m2 = std::map<int,int,std::less<int>,
        map_allocator<std::pair<const int, int>, 10>
     >{};

    for (int i = 0; i < 10; ++i)
        m2[i] = fact(i);

    for(auto const &mm : m2)
        std::cout << mm.first << " " << mm.second << std::endl; 

    Buffer<int> buffer;
    for(int i = 0; i < 10; i++)
        buffer.emplace_back(i);

    Buffer<int, map_allocator<int, 10>> buffer_2;
    for(int i = 0; i < 10; i++)
        buffer_2.emplace_back(i);
    for(auto const &buf : buffer_2)
        std::cout << *buf << std::endl;

    return 0;
}