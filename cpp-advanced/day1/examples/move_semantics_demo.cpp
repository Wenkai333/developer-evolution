// day1/examples/move_semantics_demo.cpp
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <string>

// Example 1: Understanding Move Semantics
class Buffer
{
private:
    size_t size_;
    int *data_;

public:
    // Constructor
    explicit Buffer(size_t size) : size_(size), data_(new int[size])
    {
        std::cout << "Buffer(" << size_ << ") constructed\n";
    }

    // Destructor
    ~Buffer()
    {
        delete[] data_;
        std::cout << "Buffer destroyed\n";
    }

    // Copy constructor (expensive)
    Buffer(const Buffer &other) : size_(other.size_), data_(new int[size_])
    {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "Buffer copy constructed (expensive!)\n";
    }

    // Move constructor (cheap)
    Buffer(Buffer &&other) noexcept
        : size_(other.size_), data_(other.data_)
    {
        other.size_ = 0;
        other.data_ = nullptr;
        std::cout << "Buffer move constructed (cheap!)\n";
    }

    // Copy assignment
    Buffer &operator=(const Buffer &other)
    {
        if (this != &other)
        {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
            std::cout << "Buffer copy assigned\n";
        }
        return *this;
    }

    // Move assignment
    Buffer &operator=(Buffer &&other) noexcept
    {
        if (this != &other)
        {
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
template <typename T>
void process_value(T &&val)
{
    std::cout << "Processing: ";
    if constexpr (std::is_lvalue_reference_v<T>)
    {
        std::cout << "lvalue\n";
    }
    else
    {
        std::cout << "rvalue\n";
    }
}

template <typename T, typename... Args>
auto make_unique_logged(Args &&...args)
{
    std::cout << "Creating unique_ptr with " << sizeof...(args) << " args\n";
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// Example 3: Smart Pointer Patterns
class Node : public std::enable_shared_from_this<Node>
{
public:
    int value;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent; // Avoid circular reference

    explicit Node(int val) : value(val)
    {
        std::cout << "Node " << value << " created\n";
    }

    ~Node()
    {
        std::cout << "Node " << value << " destroyed\n";
    }

    std::shared_ptr<Node> get_ptr()
    {
        return shared_from_this();
    }
};

void demonstrate_move_semantics()
{
    std::cout << "\n=== Move Semantics Demo ===\n";

    // Force move with std::move
    Buffer buf1(1000);
    Buffer buf2 = std::move(buf1); // Move constructor

    // Return value optimization (RVO)
    auto make_buffer = []()
    { return Buffer(500); };
    Buffer buf3 = make_buffer(); // Often no copy/move due to RVO

    // Moving from containers
    std::vector<Buffer> vec;
    vec.reserve(3);
    vec.emplace_back(100);      // Construct in place
    vec.push_back(Buffer(200)); // Move temporary
}

void demonstrate_smart_pointers()
{
    std::cout << "\n=== Smart Pointers Demo ===\n";

    // unique_ptr - single ownership
    {
        auto ptr1 = std::make_unique<Node>(1);
        auto ptr2 = std::move(ptr1); // Transfer ownership
        // ptr1 is now nullptr
        if (!ptr1)
            std::cout << "ptr1 is null after move\n";
    }

    // shared_ptr - shared ownership
    {
        auto shared1 = std::make_shared<Node>(2);
        {
            auto shared2 = shared1; // Reference count = 2
            std::cout << "Use count: " << shared1.use_count() << "\n";
        } // shared2 destroyed, ref count = 1
        std::cout << "Use count: " << shared1.use_count() << "\n";
    }

    // weak_ptr - non-owning reference
    {
        auto parent = std::make_shared<Node>(10);
        auto child = std::make_shared<Node>(20);

        parent->next = child;
        child->parent = parent; // weak_ptr prevents circular reference

        if (auto locked = child->parent.lock())
        {
            std::cout << "Parent value: " << locked->value << "\n";
        }
    }
}

int main()
{
    std::cout << "🚀 Day 1 Morning: Move Semantics & Smart Pointers\n";
    std::cout << "================================================\n";

    try
    {
        demonstrate_move_semantics();
        demonstrate_smart_pointers();

        std::cout << "\n✅ All demonstrations completed successfully!\n";
        std::cout << "\nNext steps:\n";
        std::cout << "1. Review the output and understand each section\n";
        std::cout << "2. Experiment with the code - modify values, add breakpoints\n";
        std::cout << "3. Move on to stl_advanced.cpp for afternoon session\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}