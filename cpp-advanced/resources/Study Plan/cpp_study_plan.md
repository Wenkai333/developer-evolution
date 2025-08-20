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
    
    if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>) {
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    } else {
        auto result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        return std::make_pair(result, 
            std::chrono::duration_cast<std::chrono::microseconds>(end - start));
    }
}

// Example 4: CRTP (Curiously Recurring Template Pattern)
template<typename Derived>
class Comparable {
public:
    bool operator!=(const Derived& other) const {
        return !static_cast<const Derived*>(this)->operator==(other);
    }
    
    bool operator<=(const Derived& other) const {
        const auto& self = static_cast<const Derived&>(*this);
        return self < other || self == other;
    }
    
    bool operator>(const Derived& other) const {
        return !static_cast<const Derived*>(this)->operator<=(other);
    }
    
    bool operator>=(const Derived& other) const {
        return !static_cast<const Derived*>(this)->operator<(other);
    }
};

class Point : public Comparable<Point> {
    double x, y;
public:
    Point(double x, double y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

// Example 5: Tag Dispatching
struct sequential_tag {};
struct parallel_tag {};

template<typename Iterator>
void sort_impl(Iterator first, Iterator last, sequential_tag) {
    std::sort(first, last);
    std::cout << "Sequential sort\n";
}

template<typename Iterator>
void sort_impl(Iterator first, Iterator last, parallel_tag) {
    std::sort(std::execution::par, first, last);
    std::cout << "Parallel sort\n";
}

template<typename Iterator>
void smart_sort(Iterator first, Iterator last) {
    const auto size = std::distance(first, last);
    if (size < 10000) {
        sort_impl(first, last, sequential_tag{});
    } else {
        sort_impl(first, last, parallel_tag{});
    }
}

// Example 6: Expression Templates
template<typename L, typename R>
class AddExpr {
    const L& lhs;
    const R& rhs;
public:
    AddExpr(const L& l, const R& r) : lhs(l), rhs(r) {}
    
    auto operator[](size_t i) const {
        return lhs[i] + rhs[i];
    }
    
    size_t size() const {
        return lhs.size();
    }
};

template<typename T>
class Vector {
    std::vector<T> data;
public:
    explicit Vector(size_t size) : data(size) {}
    Vector(std::initializer_list<T> init) : data(init) {}
    
    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
    size_t size() const { return data.size(); }
    
    template<typename Expr>
    Vector& operator=(const Expr& expr) {
        for (size_t i = 0; i < size(); ++i) {
            data[i] = expr[i];
        }
        return *this;
    }
    
    template<typename R>
    AddExpr<Vector, R> operator+(const R& rhs) const {
        return AddExpr<Vector, R>(*this, rhs);
    }
};

// Example 7: Type Traits and Concepts (C++20)
template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept Container = requires(T t) {
    typename T::value_type;
    typename T::iterator;
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
    { t.size() } -> std::convertible_to<size_t>;
};

template<Arithmetic T>
T calculate_mean(const std::vector<T>& values) {
    if (values.empty()) return T{};
    T sum = std::accumulate(values.begin(), values.end(), T{});
    return sum / static_cast<T>(values.size());
}

template<Container C>
void print_container(const C& container) {
    std::cout << "Container [" << container.size() << "]: ";
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

```

### Afternoon Session (3-4 hours): C++20 Concepts & Advanced Generic Programming

```cpp
// concepts_and_generic.cpp
#include <iostream>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <vector>
#include <span>

// Example 1: Custom Concepts
template<typename T>
concept Hashable = requires(T t) {
    { std::hash<T>{}(t) } -> std::convertible_to<size_t>;
};

template<typename T>
concept Serializable = requires(T t, std::ostream& os, std::istream& is) {
    { os << t } -> std::same_as<std::ostream&>;
    { is >> t } -> std::same_as<std::istream&>;
};

template<typename T>
concept ComparableRange = std::ranges::range<T> && 
    std::totally_ordered<std::ranges::range_value_t<T>>;

// Example 2: Constrained Templates
template<std::integral T>
class ModularInt {
    T value;
    T modulus;
public:
    ModularInt(T val, T mod) : value(val % mod), modulus(mod) {}
    
    ModularInt operator+(const ModularInt& other) const {
        return ModularInt((value + other.value) % modulus, modulus);
    }
    
    ModularInt operator*(const ModularInt& other) const {
        return ModularInt((value * other.value) % modulus, modulus);
    }
    
    T get() const { return value; }
};

// Example 3: Concept-based Overloading
template<std::integral T>
auto process(T value) {
    std::cout << "Processing integral: " << value << "\n";
    return value * 2;
}

template<std::floating_point T>
auto process(T value) {
    std::cout << "Processing floating point: " << value << "\n";
    return value * 1.5;
}

template<ComparableRange T>
auto process(T&& range) {
    std::cout << "Processing range\n";
    std::ranges::sort(range);
    return range;
}

// Example 4: Subsumption and Concept Refinement
template<typename T>
concept Drawable = requires(T t) {
    { t.draw() } -> std::same_as<void>;
};

template<typename T>
concept AnimatedDrawable = Drawable<T> && requires(T t, float dt) {
    { t.update(dt) } -> std::same_as<void>;
};

template<Drawable T>
void render(T& object) {
    std::cout << "Static render\n";
    object.draw();
}

template<AnimatedDrawable T>  // More constrained, preferred when applicable
void render(T& object) {
    std::cout << "Animated render\n";
    object.update(0.016f);
    object.draw();
}

// Example 5: Advanced Type Manipulation
template<typename T>
struct TypeIdentity {
    using type = T;
};

template<typename T>
using TypeIdentity_t = typename TypeIdentity<T>::type;

template<typename... Types>
struct TypePack {
    template<template<typename...> class Template>
    using apply = Template<Types...>;
    
    static constexpr size_t size = sizeof...(Types);
};

template<typename Pack, template<typename> class Transform>
struct TransformPack;

template<typename... Types, template<typename> class Transform>
struct TransformPack<TypePack<Types...>, Transform> {
    using type = TypePack<typename Transform<Types>::type...>;
};

// Example 6: Compile-time String Processing
template<size_t N>
struct FixedString {
    char data[N];
    
    constexpr FixedString(const char (&str)[N]) {
        std::copy_n(str, N, data);
    }
    
    constexpr size_t size() const { return N - 1; }
    constexpr const char* c_str() const { return data; }
};

template<FixedString str>
class NamedType {
public:
    static constexpr auto name() { return str.c_str(); }
};

// Usage: NamedType<"MyType"> myVar;
```

### Evening Practice Project: Generic Data Pipeline

```cpp
// data_pipeline_project.cpp
#include <iostream>
#include <vector>
#include <functional>
#include <optional>
#include <variant>
#include <any>
#include <memory>
#include <concepts>
#include <ranges>

// Generic, type-safe data processing pipeline
template<typename T>
concept Transformer = requires {
    typename T::input_type;
    typename T::output_type;
};

template<typename In, typename Out>
class Pipeline {
public:
    using input_type = In;
    using output_type = Out;
    
private:
    std::function<Out(const In&)> transform_;
    
public:
    explicit Pipeline(std::function<Out(const In&)> f) : transform_(f) {}
    
    Out operator()(const In& input) const {
        return transform_(input);
    }
    
    template<typename NextOut>
    auto then(std::function<NextOut(const Out&)> next) const {
        return Pipeline<In, NextOut>(
            [=](const In& input) { return next(transform_(input)); }
        );
    }
    
    template<Transformer T>
    requires std::same_as<typename T::input_type, Out>
    auto then(const T& transformer) const {
        using NextOut = typename T::output_type;
        return Pipeline<In, NextOut>(
            [=](const In& input) { return transformer(transform_(input)); }
        );
    }
    
    auto parallel(size_t num_threads = std::thread::hardware_concurrency()) const {
        return ParallelPipeline<In, Out>(*this, num_threads);
    }
};

template<typename In, typename Out>
class ParallelPipeline {
    Pipeline<In, Out> pipeline_;
    size_t num_threads_;
    
public:
    ParallelPipeline(Pipeline<In, Out> p, size_t threads) 
        : pipeline_(p), num_threads_(threads) {}
    
    std::vector<Out> operator()(const std::vector<In>& inputs) const {
        std::vector<Out> results(inputs.size());
        std::vector<std::thread> threads;
        
        size_t chunk_size = inputs.size() / num_threads_;
        
        for (size_t i = 0; i < num_threads_; ++i) {
            size_t start = i * chunk_size;
            size_t end = (i == num_threads_ - 1) ? inputs.size() : (i + 1) * chunk_size;
            
            threads.emplace_back([&, start, end]() {
                for (size_t j = start; j < end; ++j) {
                    results[j] = pipeline_(inputs[j]);
                }
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        return results;
    }
};

// Example transformers
struct ParseInt {
    using input_type = std::string;
    using output_type = std::optional<int>;
    
    std::optional<int> operator()(const std::string& s) const {
        try {
            return std::stoi(s);
        } catch (...) {
            return std::nullopt;
        }
    }
};

struct Square {
    using input_type = int;
    using output_type = int;
    
    int operator()(int x) const {
        return x * x;
    }
};

template<typename T>
struct Filter {
    using input_type = std::optional<T>;
    using output_type = std::optional<T>;
    
    std::function<bool(const T&)> predicate;
    
    std::optional<T> operator()(const std::optional<T>& opt) const {
        if (opt && predicate(*opt)) {
            return opt;
        }
        return std::nullopt;
    }
};

// Advanced: Type-erased pipeline
class AnyPipeline {
    struct Concept {
        virtual ~Concept() = default;
        virtual std::any apply(const std::any&) const = 0;
    };
    
    template<typename Pipeline>
    struct Model : Concept {
        Pipeline pipeline;
        
        Model(Pipeline p) : pipeline(std::move(p)) {}
        
        std::any apply(const std::any& input) const override {
            using In = typename Pipeline::input_type;
            using Out = typename Pipeline::output_type;
            
            return pipeline(std::any_cast<In>(input));
        }
    };
    
    std::unique_ptr<Concept> impl_;
    
public:
    template<typename Pipeline>
    AnyPipeline(Pipeline p) 
        : impl_(std::make_unique<Model<Pipeline>>(std::move(p))) {}
    
    std::any operator()(const std::any& input) const {
        return impl_->apply(input);
    }
};

int main() {
    // Build a pipeline
    auto pipeline = Pipeline<std::string, std::optional<int>>(
        [](const std::string& s) { return ParseInt{}(s); })
        .then(Filter<int>{[](int x) { return x > 0; }})
        .then([](const std::optional<int>& opt) {
            return opt ? std::optional<int>(*opt * *opt) : std::nullopt;
        });
    
    // Test single input
    std::string input = "42";
    auto result = pipeline(input);
    if (result) {
        std::cout << "Result: " << *result << "\n";
    }
    
    // Test parallel processing
    std::vector<std::string> inputs{"1", "2", "-3", "4", "abc", "5"};
    auto parallel = Pipeline<std::string, std::optional<int>>(
        [](const std::string& s) { return ParseInt{}(s); }).parallel();
    
    auto results = parallel(inputs);
    
    for (size_t i = 0; i < inputs.size(); ++i) {
        std::cout << inputs[i] << " -> ";
        if (results[i]) {
            std::cout << *results[i];
        } else {
            std::cout << "failed";
        }
        std::cout << "\n";
    }
    
    return 0;
}
```

---

## Day 3: Memory Management & Performance Optimization

### Morning Session (3-4 hours): Advanced Memory Management

#### Knowledge Base
**Resources:**
- [CppReference Memory Management](https://en.cppreference.com/w/cpp/memory)
- Book: "The C++ Programming Language" by Stroustrup (Ch. 34)
- [CppCon: Back to Basics - RAII](https://www.youtube.com/watch?v=7Qgd9B1KuMQ)

#### Core Concepts with Examples

```cpp
// memory_management_advanced.cpp
#include <iostream>
#include <memory>
#include <cstring>
#include <atomic>
#include <thread>
#include <vector>
#include <new>

// Example 1: Custom Memory Pool
class MemoryPool {
private:
    struct Block {
        Block* next;
    };
    
    class Chunk {
        static constexpr size_t CHUNK_SIZE = 4096;
        alignas(std::max_align_t) char memory[CHUNK_SIZE];
        size_t used = 0;
        
    public:
        void* allocate(size_t size, size_t alignment) {
            size_t space = CHUNK_SIZE - used;
            void* ptr = memory + used;
            
            if (std::align(alignment, size, ptr, space)) {
                used = CHUNK_SIZE - space + size;
                return ptr;
            }
            return nullptr;
        }
        
        bool contains(void* ptr) const {
            return ptr >= memory && ptr < memory + CHUNK_SIZE;
        }
    };
    
    std::vector<std::unique_ptr<Chunk>> chunks_;
    Block* free_list_ = nullptr;
    size_t block_size_;
    size_t alignment_;
    
public:
    MemoryPool(size_t block_size, size_t alignment = alignof(std::max_align_t))
        : block_size_(block_size), alignment_(alignment) {
        expand();
    }
    
    void* allocate() {
        if (!free_list_) {
            expand();
        }
        
        Block* block = free_list_;
        free_list_ = free_list_->next;
        return block;
    }
    
    void deallocate(void* ptr) {
        if (!ptr) return;
        
        Block* block = static_cast<Block*>(ptr);
        block->next = free_list_;
        free_list_ = block;
    }
    
private:
    void expand() {
        auto chunk = std::make_unique<Chunk>();
        
        size_t blocks_per_chunk = 4096 / block_size_;
        for (size_t i = 0; i < blocks_per_chunk; ++i) {
            void* ptr = chunk->allocate(block_size_, alignment_);
            if (ptr) {
                deallocate(ptr);
            }
        }
        
        chunks_.push_back(std::move(chunk));
    }
};

// Example 2: Reference Counting with Weak References
template<typename T>
class IntrusivePtr {
private:
    T* ptr_ = nullptr;
    
public:
    IntrusivePtr() = default;
    explicit IntrusivePtr(T* ptr) : ptr_(ptr) {
        if (ptr_) ptr_->add_ref();
    }
    
    IntrusivePtr(const IntrusivePtr& other) : ptr_(other.ptr_) {
        if (ptr_) ptr_->add_ref();
    }
    
    IntrusivePtr(IntrusivePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }
    
    ~IntrusivePtr() {
        if (ptr_) ptr_->release();
    }
    
    IntrusivePtr& operator=(const IntrusivePtr& other) {
        if (this != &other) {
            if (ptr_) ptr_->release();
            ptr_ = other.ptr_;
            if (ptr_) ptr_->add_ref();
        }
        return *this;
    }
    
    T* operator->() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* get() const { return ptr_; }
    explicit operator bool() const { return ptr_ != nullptr; }
};

class RefCounted {
private:
    mutable std::atomic<int> ref_count_{0};
    mutable std::atomic<int> weak_count_{0};
    
public:
    void add_ref() const {
        ref_count_.fetch_add(1, std::memory_order_relaxed);
    }
    
    void release() const {
        if (ref_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }
    
    void add_weak_ref() const {
        weak_count_.fetch_add(1, std::memory_order_relaxed);
    }
    
    void release_weak() const {
        weak_count_.fetch_sub(1, std::memory_order_acq_rel);
    }
    
    bool try_add_ref() const {
        int count = ref_count_.load(std::memory_order_relaxed);
        while (count != 0) {
            if (ref_count_.compare_exchange_weak(count, count + 1,
                std::memory_order_relaxed)) {
                return true;
            }
        }
        return false;
    }
    
protected:
    virtual ~RefCounted() = default;
};

// Example 3: Lock-Free Memory Management
template<typename T>
class LockFreeStack {
private:
    struct Node {
        T data;
        std::atomic<Node*> next;
        
        Node(T val) : data(std::move(val)), next(nullptr) {}
    };
    
    std::atomic<Node*> head{nullptr};
    
public:
    void push(T value) {
        Node* new_node = new Node(std::move(value));
        Node* old_head = head.load(std::memory_order_relaxed);
        
        do {
            new_node->next.store(old_head, std::memory_order_relaxed);
        } while (!head.compare_exchange_weak(old_head, new_node,
                 std::memory_order_release, std::memory_order_relaxed));
    }
    
    std::optional<T> pop() {
        Node* old_head = head.load(std::memory_order_acquire);
        
        while (old_head) {
            Node* next = old_head->next.load(std::memory_order_relaxed);
            if (head.compare_exchange_weak(old_head, next,
                std::memory_order_release, std::memory_order_acquire)) {
                T data = std::move(old_head->data);
                delete old_head;
                return data;
            }
        }
        
        return std::nullopt;
    }
    
    ~LockFreeStack() {
        while (pop()) {}
    }
};

// Example 4: Memory-Mapped Files
class MemoryMappedFile {
private:
    void* data_ = nullptr;
    size_t size_ = 0;
    int fd_ = -1;
    
public:
    MemoryMappedFile(const std::string& path, size_t size) : size_(size) {
        // Platform-specific implementation
        // This is a simplified example
#ifdef __unix__
        fd_ = open(path.c_str(), O_RDWR | O_CREAT, 0644);
        if (fd_ == -1) throw std::runtime_error("Cannot open file");
        
        if (ftruncate(fd_, size) == -1) {
            close(fd_);
            throw std::runtime_error("Cannot resize file");
        }
        
        data_ = mmap(nullptr, size, PROT_READ | PROT_WRITE, 
                     MAP_SHARED, fd_, 0);
        if (data_ == MAP_FAILED) {
            close(fd_);
            throw std::runtime_error("Cannot map file");
        }
#endif
    }
    
    ~MemoryMappedFile() {
#ifdef __unix__
        if (data_) munmap(data_, size_);
        if (fd_ != -1) close(fd_);
#endif
    }
    
    void* data() { return data_; }
    size_t size() const { return size_; }
};
```

### Afternoon Session (3-4 hours): Performance Optimization

```cpp
// performance_optimization.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <immintrin.h>  // For SIMD
#include <cstring>

// Example 1: Cache-Friendly Data Structures
// AoS (Array of Structures) - Poor cache utilization
struct ParticleAoS {
    float x, y, z;
    float vx, vy, vz;
    float mass;
    float charge;
};

// SoA (Structure of Arrays) - Better cache utilization
struct ParticlesSoA {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> mass;
    std::vector<float> charge;
    
    void resize(size_t n) {
        x.resize(n); y.resize(n); z.resize(n);
        vx.resize(n); vy.resize(n); vz.resize(n);
        mass.resize(n); charge.resize(n);
    }
};

// Example 2: SIMD Optimization
class SimdVector {
private:
    float* data_;
    size_t size_;
    size_t capacity_;
    
public:
    SimdVector(size_t size) : size_(size) {
        // Align to 32 bytes for AVX
        capacity_ = (size + 7) & ~7;  // Round up to multiple of 8
        data_ = static_cast<float*>(std::aligned_alloc(32, capacity_ * sizeof(float)));
        std::memset(data_, 0, capacity_ * sizeof(float));
    }
    
    ~SimdVector() {
        std::free(data_);
    }
    
    // Vectorized addition using AVX
    void add(const SimdVector& other) {
        size_t simd_size = size_ & ~7;  // Process 8 floats at a time
        
        for (size_t i = 0; i < simd_size; i += 8) {
            __m256 a = _mm256_load_ps(&data_[i]);
            __m256 b = _mm256_load_ps(&other.data_[i]);
            __m256 result = _mm256_add_ps(a, b);
            _mm256_store_ps(&data_[i], result);
        }
        
        // Handle remaining elements
        for (size_t i = simd_size; i < size_; ++i) {
            data_[i] += other.data_[i];
        }
    }
    
### Evening Practice Project: High-Performance Matrix Library

```cpp
// matrix_library_project.cpp
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <immintrin.h>
#include <thread>
#include <future>

// High-performance matrix class with expression templates and SIMD
template<typename T>
class Matrix {
private:
    std::unique_ptr<T[], std::function<void(T*)>> data_;
    size_t rows_, cols_;
    size_t stride_;  // For submatrix views
    
    static T* allocate_aligned(size_t size) {
        return static_cast<T*>(std::aligned_alloc(32, size * sizeof(T)));
    }
    
public:
    Matrix(size_t rows, size_t cols) 
        : rows_(rows), cols_(cols), stride_(cols),
          data_(allocate_aligned(rows * cols), 
                [](T* p) { std::free(p); }) {
        std::fill_n(data_.get(), rows_ * cols_, T{});
    }
    
    // Copy constructor
    Matrix(const Matrix& other) 
        : rows_(other.rows_), cols_(other.cols_), stride_(cols_),
          data_(allocate_aligned(rows_ * cols_),
                [](T* p) { std::free(p); }) {
        std::copy_n(other.data_.get(), rows_ * cols_, data_.get());
    }
    
    // Move constructor
    Matrix(Matrix&&) = default;
    
    // Element access
    T& operator()(size_t i, size_t j) {
        return data_[i * stride_ + j];
    }
    
    const T& operator()(size_t i, size_t j) const {
        return data_[i * stride_ + j];
    }
    
    // Submatrix view (no copy)
    Matrix submatrix(size_t row_start, size_t col_start, 
                     size_t rows, size_t cols) {
        Matrix sub(0, 0);  // Empty matrix
        sub.rows_ = rows;
        sub.cols_ = cols;
        sub.stride_ = stride_;
        sub.data_ = std::shared_ptr<T[]>(data_, 
                    &data_[row_start * stride_ + col_start]);
        return sub;
    }
    
    // Optimized matrix multiplication
    Matrix operator*(const Matrix& other) const {
        if (cols_ != other.rows_) {
            throw std::invalid_argument("Matrix dimensions don't match");
        }
        
        Matrix result(rows_, other.cols_);
        
        // Cache blocking for better performance
        const size_t block_size = 64;
        
        #pragma omp parallel for collapse(2)
        for (size_t ii = 0; ii < rows_; ii += block_size) {
            for (size_t jj = 0; jj < other.cols_; jj += block_size) {
                for (size_t kk = 0; kk < cols_; kk += block_size) {
                    // Process block
                    for (size_t i = ii; i < std::min(ii + block_size, rows_); ++i) {
                        for (size_t j = jj; j < std::min(jj + block_size, other.cols_); ++j) {
                            T sum = 0;
                            for (size_t k = kk; k < std::min(kk + block_size, cols_); ++k) {
                                sum += (*this)(i, k) * other(k, j);
                            }
                            result(i, j) += sum;
                        }
                    }
                }
            }
        }
        
        return result;
    }
    
    // SIMD-optimized addition
    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions don't match");
        }
        
        Matrix result(rows_, cols_);
        size_t total = rows_ * cols_;
        
        if constexpr (std::is_same_v<T, float>) {
            size_t simd_size = total & ~7;
            
            for (size_t i = 0; i < simd_size; i += 8) {
                __m256 a = _mm256_load_ps(&data_[i]);
                __m256 b = _mm256_load_ps(&other.data_[i]);
                __m256 sum = _mm256_add_ps(a, b);
                _mm256_store_ps(&result.data_[i], sum);
            }
            
            for (size_t i = simd_size; i < total; ++i) {
                result.data_[i] = data_[i] + other.data_[i];
            }
        } else {
            std::transform(data_.get(), data_.get() + total,
                          other.data_.get(), result.data_.get(),
                          std::plus<T>());
        }
        
        return result;
    }
    
    // Transpose (cache-friendly)
    Matrix transpose() const {
        Matrix result(cols_, rows_);
        const size_t block_size = 32;
        
        for (size_t i = 0; i < rows_; i += block_size) {
            for (size_t j = 0; j < cols_; j += block_size) {
                // Transpose block
                for (size_t bi = i; bi < std::min(i + block_size, rows_); ++bi) {
                    for (size_t bj = j; bj < std::min(j + block_size, cols_); ++bj) {
                        result(bj, bi) = (*this)(bi, bj);
                    }
                }
            }
        }
        
        return result;
    }
    
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
};

// Expression template for lazy evaluation
template<typename Expr>
class MatrixExpression {
    const Expr& expr_;
public:
    explicit MatrixExpression(const Expr& expr) : expr_(expr) {}
    
    auto operator()(size_t i, size_t j) const {
        return expr_(i, j);
    }
    
    size_t rows() const { return expr_.rows(); }
    size_t cols() const { return expr_.cols(); }
};

template<typename L, typename R>
class MatrixSum {
    const L& lhs_;
    const R& rhs_;
public:
    MatrixSum(const L& l, const R& r) : lhs_(l), rhs_(r) {}
    
    auto operator()(size_t i, size_t j) const {
        return lhs_(i, j) + rhs_(i, j);
    }
    
    size_t rows() const { return lhs_.rows(); }
    size_t cols() const { return lhs_.cols(); }
};

// Benchmark utilities
template<typename Func>
auto benchmark(const std::string& name, Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    auto result = func();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << name << ": " << duration.count() << " μs\n";
    
    return result;
}

int main() {
    const size_t size = 512;
    
    std::cout << "=== Matrix Performance Benchmark ===\n";
    
    // Initialize matrices
    Matrix<float> A(size, size);
    Matrix<float> B(size, size);
    
    // Fill with random data
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            A(i, j) = static_cast<float>(rand()) / RAND_MAX;
            B(i, j) = static_cast<float>(rand()) / RAND_MAX;
        }
    }
    
    // Benchmark operations
    auto C = benchmark("Matrix Addition (SIMD)", [&]() {
        return A + B;
    });
    
    auto D = benchmark("Matrix Multiplication (Blocked)", [&]() {
        return A * B;
    });
    
    auto E = benchmark("Matrix Transpose (Cache-friendly)", [&]() {
        return A.transpose();
    });
    
    // Test submatrix view
    auto sub = benchmark("Submatrix View", [&]() {
        return A.submatrix(100, 100, 200, 200);
    });
    
    std::cout << "\nMatrix dimensions:\n";
    std::cout << "A: " << A.rows() << "x" << A.cols() << "\n";
    std::cout << "Submatrix: " << sub.rows() << "x" << sub.cols() << "\n";
    
    return 0;
}
```

## Study Schedule & Daily Practice

### Day 1 Schedule
- **9:00-12:00**: Move Semantics & Smart Pointers
  - Read theory and run examples
  - Modify examples to test understanding
  - Write your own RAII wrapper class
  
- **13:00-16:00**: STL Advanced Patterns
  - Study ranges and algorithms
  - Practice with custom allocators
  - Implement a custom container
  
- **16:30-19:00**: Resource Manager Project
  - Implement the full project
  - Add features (serialization, statistics)
  - Profile and optimize

### Day 2 Schedule  
- **9:00-12:00**: Template Metaprogramming
  - Study SFINAE, concepts, CRTP
  - Write compile-time utilities
  - Practice with expression templates
  
- **13:00-16:00**: C++20 Concepts
  - Define custom concepts
  - Constrain templates properly
  - Study subsumption rules
  
- **16:30-19:00**: Data Pipeline Project
  - Build the pipeline system
  - Add error handling
  - Create performance benchmarks

### Day 3 Schedule
- **9:00-12:00**: Memory Management
  - Study allocators and pools
  - Implement lock-free structures
  - Practice with smart pointers
  
- **13:00-16:00**: Performance Optimization
  - Learn SIMD programming
  - Study cache optimization
  - Profile code with tools
  
- **16:30-19:00**: Matrix Library Project
  - Implement core functionality
  - Add SIMD optimizations
  - Benchmark against existing libraries

## Key Learning Objectives

### After Day 1
- Master move semantics and perfect forwarding
- Understand smart pointer internals
- Use STL algorithms effectively
- Build resource-aware applications

### After Day 2
- Write complex template metaprograms
- Use concepts for better APIs
- Understand compile-time programming
- Build generic, reusable components

### After Day 3
- Implement custom memory management
- Optimize for CPU cache and SIMD
- Profile and improve performance
- Build high-performance libraries

## Additional Resources

### Books
- "Effective Modern C++" - Scott Meyers
- "C++ Templates" - Vandevoorde & Josuttis
- "C++ Concurrency in Action" - Anthony Williams
- "The C++ Standard Library" - Josuttis

### Online
- [CppReference](https://en.cppreference.com)
- [Compiler Explorer](https://godbolt.org)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [CppCon YouTube Channel](https://www.youtube.com/user/CppCon)

### Tools
- Valgrind (memory debugging)
- Perf (Linux profiling)
- AddressSanitizer (memory errors)
- Clang-tidy (static analysis)

## Practice Challenges

1. **Memory Pool Challenge**: Implement a thread-safe memory pool with statistics
2. **Template Challenge**: Create a compile-time JSON parser
3. **Performance Challenge**: Optimize a ray tracer using SIMD
4. **STL Challenge**: Implement your own `std::function` replacement
5. **Concurrency Challenge**: Build a lock-free queue

Remember to compile with optimization flags (`-O3 -march=native`) for performance testing and debug flags (`-g -fsanitize=address,undefined`) for development!
            