# 3-Day Advanced C++ Study Plan

## Day 1: Modern C++ Standards & STL Mastery

### Morning Session (3-4 hours): Move Semantics & Smart Pointers

#### Knowledge Base
**Resources:**
- [CppReference Move Semantics](https://en.cppreference.com/w/cpp/language/move_constructor)
- Book: "Effective Modern C++" by Scott Meyers (Items 23-30)
- [Back to Basics: Move Semantics](https://www.youtube.com/watch?v=St0MNEU5b0o) - CppCon

#### Core Concepts with Examples

```cpp
// move_semantics_demo.cpp
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <string>

// Example 1: Understanding Move Semantics
class Buffer {
private:
    size_t size_;
    int* data_;
    
public:
    // Constructor
    explicit Buffer(size_t size) : size_(size), data_(new int[size]) {
        std::cout << "Buffer(" << size_ << ") constructed\n";
    }
    
    // Destructor
    ~Buffer() {
        delete[] data_;
        std::cout << "Buffer destroyed\n";
    }
    
    // Copy constructor (expensive)
    Buffer(const Buffer& other) : size_(other.size_), data_(new int[size_]) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "Buffer copy constructed (expensive!)\n";
    }
    
    // Move constructor (cheap)
    Buffer(Buffer&& other) noexcept 
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        std::cout << "Buffer move constructed (cheap!)\n";
    }
    
    // Copy assignment
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
            std::cout << "Buffer copy assigned\n";
        }
        return *this;
    }
    
    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
            std::cout << "Buffer move assigned\n";
        }
        return *this;
    }
};

// Example 2: Perfect Forwarding
template<typename T>
void process_value(T&& val) {
    std::cout << "Processing: ";
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "lvalue\n";
    } else {
        std::cout << "rvalue\n";
    }
}

template<typename T, typename... Args>
auto make_unique_logged(Args&&... args) {
    std::cout << "Creating unique_ptr with " << sizeof...(args) << " args\n";
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// Example 3: Smart Pointer Patterns
class Node : public std::enable_shared_from_this<Node> {
public:
    int value;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent;  // Avoid circular reference
    
    explicit Node(int val) : value(val) {
        std::cout << "Node " << value << " created\n";
    }
    
    ~Node() {
        std::cout << "Node " << value << " destroyed\n";
    }
    
    std::shared_ptr<Node> get_ptr() {
        return shared_from_this();
    }
};

void demonstrate_move_semantics() {
    std::cout << "\n=== Move Semantics Demo ===\n";
    
    // Force move with std::move
    Buffer buf1(1000);
    Buffer buf2 = std::move(buf1);  // Move constructor
    
    // Return value optimization (RVO)
    auto make_buffer = []() { return Buffer(500); };
    Buffer buf3 = make_buffer();  // Often no copy/move due to RVO
    
    // Moving from containers
    std::vector<Buffer> vec;
    vec.reserve(3);
    vec.emplace_back(100);  // Construct in place
    vec.push_back(Buffer(200));  // Move temporary
}

void demonstrate_smart_pointers() {
    std::cout << "\n=== Smart Pointers Demo ===\n";
    
    // unique_ptr - single ownership
    {
        auto ptr1 = std::make_unique<Node>(1);
        auto ptr2 = std::move(ptr1);  // Transfer ownership
        // ptr1 is now nullptr
        if (!ptr1) std::cout << "ptr1 is null after move\n";
    }
    
    // shared_ptr - shared ownership
    {
        auto shared1 = std::make_shared<Node>(2);
        {
            auto shared2 = shared1;  // Reference count = 2
            std::cout << "Use count: " << shared1.use_count() << "\n";
        }  // shared2 destroyed, ref count = 1
        std::cout << "Use count: " << shared1.use_count() << "\n";
    }
    
    // weak_ptr - non-owning reference
    {
        auto parent = std::make_shared<Node>(10);
        auto child = std::make_shared<Node>(20);
        
        parent->next = child;
        child->parent = parent;  // weak_ptr prevents circular reference
        
        if (auto locked = child->parent.lock()) {
            std::cout << "Parent value: " << locked->value << "\n";
        }
    }
}
```

### Afternoon Session (3-4 hours): STL Algorithms & Containers

#### Advanced STL Patterns

```cpp
// stl_advanced.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <set>
#include <execution>
#include <functional>
#include <span>

// Example 1: C++20 Ranges
void ranges_demo() {
    std::cout << "\n=== Ranges Demo ===\n";
    
    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Traditional approach
    std::vector<int> result1;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(result1),
                 [](int n) { return n % 2 == 0; });
    std::transform(result1.begin(), result1.end(), result1.begin(),
                   [](int n) { return n * n; });
    
    // Ranges approach (C++20)
    auto result2 = nums 
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; });
    
    std::cout << "Squared evens: ";
    for (int n : result2) {
        std::cout << n << " ";
    }
    std::cout << "\n";
    
    // Lazy evaluation
    auto infinite = std::views::iota(1)  // Infinite sequence
        | std::views::transform([](int n) { return n * n; })
        | std::views::take(10);
    
    std::cout << "First 10 squares: ";
    for (int n : infinite) {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

// Example 2: Custom Allocator
template<typename T>
class PoolAllocator {
private:
    struct Block {
        alignas(T) char data[sizeof(T)];
        Block* next;
    };
    
    Block* free_list = nullptr;
    std::vector<std::unique_ptr<Block[]>> pools;
    size_t pool_size = 1024;
    
public:
    using value_type = T;
    
    PoolAllocator() = default;
    
    template<typename U>
    PoolAllocator(const PoolAllocator<U>&) {}
    
    T* allocate(size_t n) {
        if (n != 1) {
            return static_cast<T*>(::operator new(n * sizeof(T)));
        }
        
        if (!free_list) {
            expand_pool();
        }
        
        Block* block = free_list;
        free_list = free_list->next;
        return reinterpret_cast<T*>(block);
    }
    
    void deallocate(T* ptr, size_t n) {
        if (n != 1) {
            ::operator delete(ptr);
            return;
        }
        
        Block* block = reinterpret_cast<Block*>(ptr);
        block->next = free_list;
        free_list = block;
    }
    
private:
    void expand_pool() {
        auto new_pool = std::make_unique<Block[]>(pool_size);
        for (size_t i = 0; i < pool_size - 1; ++i) {
            new_pool[i].next = &new_pool[i + 1];
        }
        new_pool[pool_size - 1].next = free_list;
        free_list = &new_pool[0];
        pools.push_back(std::move(new_pool));
    }
};

// Example 3: Advanced Container Usage
void container_tricks() {
    std::cout << "\n=== Container Tricks ===\n";
    
    // 1. Heterogeneous lookup in ordered containers (C++14)
    std::set<std::string, std::less<>> words{"apple", "banana", "cherry"};
    
    // Can search with string_view without creating std::string
    std::string_view sv = "banana";
    if (words.find(sv) != words.end()) {
        std::cout << "Found: " << sv << "\n";
    }
    
    // 2. Node extraction and insertion (C++17)
    std::map<int, std::string> map1{{1, "one"}, {2, "two"}};
    std::map<int, std::string> map2{{3, "three"}};
    
    auto node = map1.extract(2);
    if (!node.empty()) {
        node.key() = 4;  // Modify key without reallocation
        map2.insert(std::move(node));
    }
    
    // 3. try_emplace and insert_or_assign (C++17)
    std::unordered_map<int, std::string> cache;
    
    // Only constructs if key doesn't exist
    auto [it1, inserted1] = cache.try_emplace(1, "value1");
    auto [it2, inserted2] = cache.try_emplace(1, "value2");  // Not inserted
    
    std::cout << "Inserted first: " << inserted1 << ", second: " << inserted2 << "\n";
    
    // 4. std::span for non-owning views (C++20)
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::span<int> view(vec.data() + 1, 3);  // View of elements 2, 3, 4
    
    for (int& val : view) {
        val *= 2;  // Modifies original vector
    }
}

// Example 4: Parallel STL (C++17)
void parallel_algorithms() {
    std::cout << "\n=== Parallel Algorithms ===\n";
    
    std::vector<double> data(1'000'000);
    std::iota(data.begin(), data.end(), 1.0);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Sequential
    double sum_seq = std::accumulate(data.begin(), data.end(), 0.0);
    
    auto mid = std::chrono::high_resolution_clock::now();
    
    // Parallel
    double sum_par = std::reduce(std::execution::par_unseq, 
                                  data.begin(), data.end(), 0.0);
    
    auto end = std::chrono::high_resolution_clock::now();
    
    auto seq_time = std::chrono::duration_cast<std::chrono::microseconds>(mid - start);
    auto par_time = std::chrono::duration_cast<std::chrono::microseconds>(end - mid);
    
    std::cout << "Sequential: " << seq_time.count() << "μs\n";
    std::cout << "Parallel: " << par_time.count() << "μs\n";
    std::cout << "Speedup: " << static_cast<double>(seq_time.count()) / par_time.count() << "x\n";
}
```

### Evening Practice Project: Resource Manager

```cpp
// resource_manager_project.cpp
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <optional>
#include <variant>
#include <filesystem>

// Generic resource manager using modern C++ features
template<typename Resource>
class ResourceManager {
private:
    struct ResourceData {
        std::shared_ptr<Resource> resource;
        std::chrono::steady_clock::time_point last_access;
        size_t access_count = 0;
    };
    
    std::unordered_map<std::string, ResourceData> cache_;
    std::function<std::unique_ptr<Resource>(const std::string&)> loader_;
    size_t max_cache_size_;
    
public:
    ResourceManager(size_t max_size, 
                   std::function<std::unique_ptr<Resource>(const std::string&)> loader)
        : max_cache_size_(max_size), loader_(std::move(loader)) {}
    
    std::shared_ptr<Resource> get(const std::string& path) {
        auto it = cache_.find(path);
        
        if (it != cache_.end()) {
            // Update access info
            it->second.last_access = std::chrono::steady_clock::now();
            it->second.access_count++;
            return it->second.resource;
        }
        
        // Load new resource
        if (cache_.size() >= max_cache_size_) {
            evict_lru();
        }
        
        auto resource = loader_(path);
        if (!resource) return nullptr;
        
        auto shared = std::shared_ptr<Resource>(std::move(resource));
        cache_[path] = {
            shared, 
            std::chrono::steady_clock::now(),
            1
        };
        
        return shared;
    }
    
    void clear() { cache_.clear(); }
    
    size_t size() const { return cache_.size(); }
    
private:
    void evict_lru() {
        auto oldest = cache_.begin();
        auto oldest_time = oldest->second.last_access;
        
        for (auto it = cache_.begin(); it != cache_.end(); ++it) {
            if (it->second.last_access < oldest_time) {
                oldest = it;
                oldest_time = it->second.last_access;
            }
        }
        
        cache_.erase(oldest);
    }
};

// Example resources
struct Texture {
    std::vector<uint8_t> data;
    int width, height;
    
    explicit Texture(const std::string& path) {
        // Simulate loading
        std::cout << "Loading texture: " << path << "\n";
        width = 1024;
        height = 1024;
        data.resize(width * height * 4);
    }
};

struct Sound {
    std::vector<int16_t> samples;
    int sample_rate;
    
    explicit Sound(const std::string& path) {
        std::cout << "Loading sound: " << path << "\n";
        sample_rate = 44100;
        samples.resize(sample_rate * 2);  // 2 seconds
    }
};

// Variant-based multi-type resource manager
using AnyResource = std::variant<Texture, Sound>;

class MultiResourceManager {
private:
    ResourceManager<AnyResource> manager_;
    
public:
    MultiResourceManager(size_t max_size) 
        : manager_(max_size, [this](const std::string& path) {
            return this->load_resource(path);
        }) {}
    
    template<typename T>
    std::shared_ptr<T> get(const std::string& path) {
        auto resource = manager_.get(path);
        if (!resource) return nullptr;
        
        if (auto* ptr = std::get_if<T>(resource.get())) {
            return std::shared_ptr<T>(resource, ptr);
        }
        
        return nullptr;
    }
    
private:
    std::unique_ptr<AnyResource> load_resource(const std::string& path) {
        namespace fs = std::filesystem;
        
        auto ext = fs::path(path).extension();
        
        if (ext == ".png" || ext == ".jpg") {
            return std::make_unique<AnyResource>(Texture(path));
        } else if (ext == ".wav" || ext == ".mp3") {
            return std::make_unique<AnyResource>(Sound(path));
        }
        
        return nullptr;
    }
};

int main() {
    // Test the resource manager
    MultiResourceManager manager(3);
    
    auto tex1 = manager.get<Texture>("image1.png");
    auto tex2 = manager.get<Texture>("image2.png");
    auto sound1 = manager.get<Sound>("sound1.wav");
    auto tex3 = manager.get<Texture>("image3.png");
    
    // This should trigger eviction
    auto tex4 = manager.get<Texture>("image4.png");
    
    // Try to get evicted resource (will reload)
    auto tex1_again = manager.get<Texture>("image1.png");
    
    return 0;
}
```

---

## Day 2: Template Metaprogramming & Generic Programming

### Morning Session (3-4 hours): Advanced Templates

#### Knowledge Base
**Resources:**
- [Template Metaprogramming](https://en.cppreference.com/w/cpp/language/templates)
- Book: "C++ Templates: The Complete Guide" by Vandevoorde, Josuttis, Gregor
- [CppCon: Template Metaprogramming](https://www.youtube.com/watch?v=Am2is2QCvxY)

#### Core Concepts with Examples

```cpp
// template_metaprogramming.cpp
#include <iostream>
#include <type_traits>
#include <tuple>
#include <array>
#include <concepts>

// Example 1: SFINAE and enable_if
template<typename T>
using EnableIfIntegral = std::enable_if_t<std::is_integral_v<T>, int>;

template<typename T, EnableIfIntegral<T> = 0>
T safe_divide(T a, T b) {
    return b != 0 ? a / b : 0;
}

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
safe_divide(T a, T b) {
    return b != 0.0 ? a / b : 0.0;
}

// Example 2: Compile-time computation with constexpr
template<size_t N>
struct Fibonacci {
    static constexpr size_t value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template<>
struct Fibonacci<0> {
    static constexpr size_t value = 0;
};

template<>
struct Fibonacci<1> {
    static constexpr size_t value = 1;
};

// Modern approach with constexpr function
template<size_t N>
constexpr size_t fibonacci() {
    if constexpr (N == 0) return 0;
    else if constexpr (N == 1) return 1;
    else return fibonacci<N-1>() + fibonacci<N-2>();
}

// Example 3: Variadic Templates and Perfect Forwarding
template<typename... Args>
class TypeList {
public:
    static constexpr size_t size = sizeof...(Args);
    
    template<size_t I>
    using type = std::tuple_element_t<I, std::tuple<Args...>>;
};

template<typename Func, typename... Args>
auto time_execution(Func&& func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    
    if constexpr (std::is_void_v<std::invoke_result