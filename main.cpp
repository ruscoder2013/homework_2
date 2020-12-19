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
    }
    ~map_allocator() {
        if (mem_p) {
            std::free(mem_p);
            mem_p = nullptr;
        }
    }

    template<typename U, int M> 
    map_allocator(const map_allocator<U, M>&) {}

    void* mem_p;
    int next_mem;

    T *allocate(std::size_t n) {
        void* addr = 0;
        if ((next_mem+n)>N)
            throw std::bad_alloc();
        if (mem_p == nullptr)
        {
            mem_p = std::malloc(N * sizeof(T));
            if (!mem_p)
                throw std::bad_alloc();
            addr = mem_p;
            next_mem = n;
        }
        else {
            addr = reinterpret_cast<T *>(mem_p) + next_mem;
            next_mem += n;
        }
        return reinterpret_cast<T *>(addr);
    }

    void deallocate(T *p, std::size_t n) {
        (void)p;
        (void)n;
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(T *p) {
        p->~T();
    }
};

constexpr int fact(int n) {
    int res = 1;
    for (int i = 1; i <= n; i++) {
	    res = res * i;
    }
    return res;
}

template<class T, class Alloc = std::allocator<T>>
class Buffer {
public:
    Buffer(unsigned int size) {
        data = std::allocator_traits<Alloc>::allocate(alloc, size);
		next_elem = 0;
    }
    
    ~Buffer()
    {
      std::allocator_traits<Alloc>::destroy(alloc, data);
      std::allocator_traits<Alloc>::deallocate(alloc, data, 1);
    }
    
    void add(T elem) {
        std::allocator_traits<Alloc>::construct(alloc, &data[next_elem], elem);
		next_elem++;
    }
	
	void show_all() {
		for(int i = 0; i < next_elem; i++)
			std::cout << data[i] << std::endl;
	}
	
	unsigned int get_size() {
	    return next_elem;
	}
	
	T* at(int n) {
	    return &data[n];
	}
	
private:
    typename std::allocator_traits<Alloc>::pointer data;
    Alloc alloc;
	int next_elem;
	int size;
};

int main() {
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

    Buffer<int, map_allocator<int, 10>> buffer(10);
    for (int i = 0; i < 10; ++i)
        buffer.add(fact(i));
    for(unsigned int i = 0; i < buffer.get_size(); i++)
        std::cout << *buffer.at(i) << std::endl;

    Buffer<int> buffer_2(10);
    for (int i = 0; i < 10; ++i)
        buffer_2.add(fact(i));
    for(unsigned int i = 0; i < buffer_2.get_size(); i++)
        std::cout << *buffer_2.at(i) << std::endl;
}
