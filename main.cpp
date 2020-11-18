#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <memory>
#include <cstring>
#include "map_allocator.h"

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
