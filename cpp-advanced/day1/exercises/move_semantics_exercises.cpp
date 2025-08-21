// day1/exercises/move_semantics_exercises.cpp
// Move Semantics Hands-On Exercises

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <algorithm>

// =============================================================================
// Exercise 1: Fix the String Wrapper Class
// =============================================================================

class StringWrapper
{
private:
    std::string *data_;

public:
    // Constructor
    explicit StringWrapper(const std::string &str)
    {
        data_ = new std::string(str);
        std::cout << "StringWrapper constructed with: " << *data_ << std::endl;
    }

    // TODO: Implement destructor
    // Guide: Clean up the dynamically allocated string
    // Hint: delete data_; and set to nullptr
    ~StringWrapper()
    {
        if (data_ != nullptr)
        {
            delete data_;
            data_ = nullptr;
        }
    }

    // TODO: Implement copy constructor
    // Guide: Create a deep copy of the string data
    // Hint: data_ = new std::string(*other.data_);
    // Don't forget to print a message!
    StringWrapper(const StringWrapper &other)
    {
        data_ = new std::string(*(other.data_));
        std::cout << "StringWrapper copy constructed with: " << *data_ << std::endl;
    }

    // TODO: Implement move constructor
    // Guide: Steal the data pointer from other, leave other in valid state
    // Hint: Take other's data_, set other's data_ to nullptr
    // This should be noexcept!
    StringWrapper(StringWrapper &&other) noexcept
    {
        data_ = other.data_;
        other.data_ = nullptr;
        std::cout << "StringWrapper move constructed with: " << *data_ << std::endl;
    }

    // TODO: Implement copy assignment operator
    // Guide: 1. Check for self-assignment, 2. Delete current data, 3. Copy from other
    // Hint: if (this != &other) { delete data_; data_ = new std::string(*other.data_); }
    StringWrapper &operator=(const StringWrapper &other)
    {
        if (this != &other)
        {                                            // Self-assignment check
            delete data_;                            // Clean up current resource
            data_ = new std::string(*(other.data_)); // Deep copy
        }
        std::cout << "StringWrapper copy assigned with: " << *data_ << std::endl;
        return *this;
    }

    // TODO: Implement move assignment operator
    // Guide: 1. Check for self-assignment, 2. Delete current data, 3. Steal from other
    // Hint: Similar to move constructor but need to clean up first
    // This should be noexcept!
    StringWrapper &operator=(StringWrapper &&other) noexcept
    {
        if (this != &other)
        {
            delete data_;
            data_ = other.data_;
            other.data_ = nullptr;
        }
        std::cout << "StringWrapper move assign opt with: " << *data_ << std::endl;
        return *this;
    }

    // Utility methods (already implemented)
    const std::string &
    get() const
    {
        return *data_;
    }
    void set(const std::string &str) { *data_ = str; }

    void print_info() const
    {
        std::cout << "StringWrapper contains: \"" << *data_
                  << "\" at address: " << static_cast<void *>(data_) << std::endl;
    }
};

void test_string_wrapper()
{
    std::cout << "\n=== Exercise 1: StringWrapper Test ===\n";

    // Test basic construction
    StringWrapper sw1("Hello");
    sw1.print_info();

    // TODO: Test copy constructor - should see "copy constructed" message
    // Guide: Create sw2 as a copy of sw1
    // Hint: StringWrapper sw2 = sw1;
    StringWrapper sw2(sw1);

    // TODO: Test move constructor - should see "move constructed" message
    // Guide: Create sw3 by moving from sw1
    // Hint: StringWrapper sw3 = std::move(sw1);
    StringWrapper sw3(std::move(sw1));

    // TODO: Test copy assignment
    // Guide: Create sw4, then assign sw2 to it
    // Hint: StringWrapper sw4("temp"); sw4 = sw2;
    StringWrapper sw4("temp");
    sw4 = sw2;

    // TODO: Test move assignment
    // Guide: Move sw2 into sw4
    // Hint: sw4 = std::move(sw2);
    sw4 = std::move(sw2);

    std::cout << "Exercise 1 completed!\n";
}

// =============================================================================
// Exercise 2: Performance Comparison - Copy vs Move
// =============================================================================

class HeavyResource
{
private:
    std::vector<double> data_;
    std::string name_;

public:
    explicit HeavyResource(size_t size, const std::string &name = "Resource")
        : data_(size, 3.14159), name_(name)
    {
        std::cout << "HeavyResource '" << name_ << "' created with " << size << " elements\n";
    }

    // TODO: Implement copy constructor
    // Guide: Copy both data_ and name_ from other
    // Hint: data_(other.data_), name_(other.name_)
    // Add timing to see how long it takes!

    // TODO: Implement move constructor
    // Guide: Move both data_ and name_ from other, mark as noexcept
    // Hint: data_(std::move(other.data_)), name_(std::move(other.name_))
    // Add timing to see how fast it is!

    // TODO: Implement copy assignment
    // Guide: Check self-assignment, then copy data and name

    // TODO: Implement move assignment
    // Guide: Check self-assignment, then move data and name

    ~HeavyResource()
    {
        std::cout << "HeavyResource '" << name_ << "' destroyed\n";
    }

    size_t size() const { return data_.size(); }
    const std::string &name() const { return name_; }
};

void performance_test()
{
    std::cout << "\n=== Exercise 2: Performance Test ===\n";

    const size_t ITERATIONS = 1000;
    const size_t RESOURCE_SIZE = 10000;

    // TODO: Test copy performance
    // Guide: 1. Start timer, 2. Create vector and copy resources ITERATIONS times, 3. End timer
    // Hint: Use std::chrono::high_resolution_clock

    auto start_copy = std::chrono::high_resolution_clock::now();
    {
        std::vector<HeavyResource> resources;
        resources.reserve(ITERATIONS);

        for (size_t i = 0; i < ITERATIONS; ++i)
        {
            HeavyResource temp(RESOURCE_SIZE, "Copy" + std::to_string(i));
            // TODO: Push temp using copy (not move)
            // Hint: resources.push_back(temp);  // This copies!
        }
    }
    auto end_copy = std::chrono::high_resolution_clock::now();

    // TODO: Test move performance
    // Guide: Same as above but use std::move when pushing

    auto start_move = std::chrono::high_resolution_clock::now();
    {
        std::vector<HeavyResource> resources;
        resources.reserve(ITERATIONS);

        for (size_t i = 0; i < ITERATIONS; ++i)
        {
            HeavyResource temp(RESOURCE_SIZE, "Move" + std::to_string(i));
            // TODO: Push temp using move
            // Hint: resources.push_back(std::move(temp));
        }
    }
    auto end_move = std::chrono::high_resolution_clock::now();

    // TODO: Calculate and print timing results
    // Guide: Use duration_cast to get milliseconds, compare copy vs move times

    std::cout << "Performance test completed!\n";
}

// =============================================================================
// Exercise 3: Smart Pointer Workshop
// =============================================================================

class Resource
{
public:
    int id;
    std::string data;

    Resource(int i, const std::string &d) : id(i), data(d)
    {
        std::cout << "Resource " << id << " created\n";
    }

    ~Resource()
    {
        std::cout << "Resource " << id << " destroyed\n";
    }
};

void smart_pointer_exercises()
{
    std::cout << "\n=== Exercise 3: Smart Pointer Workshop ===\n";

    // TODO: Create a unique_ptr to Resource
    // Guide: Use std::make_unique to create Resource with id=1, data="unique"
    // Hint: auto ptr1 = std::make_unique<Resource>(1, "unique");

    // TODO: Transfer ownership of ptr1 to ptr2 using move
    // Guide: Create ptr2 and move ptr1 into it
    // Hint: auto ptr2 = std::move(ptr1);

    // TODO: Verify ptr1 is now null
    // Guide: Check if ptr1 is nullptr and print result

    // TODO: Create shared_ptr and demonstrate reference counting
    // Guide: Create shared1 with Resource(2, "shared")
    // Hint: auto shared1 = std::make_shared<Resource>(2, "shared");

    // TODO: Create shared2 and shared3 pointing to same resource
    // Guide: Copy shared1 to create shared2 and shared3
    // Print use_count() after each copy

    // TODO: Create weak_ptr to break potential cycles
    // Guide: Create weak1 from shared1
    // Hint: std::weak_ptr<Resource> weak1 = shared1;

    // TODO: Demonstrate weak_ptr.lock()
    // Guide: Use weak1.lock() to get temporary shared_ptr, check if it's valid

    std::cout << "Smart pointer exercises completed!\n";
}

// =============================================================================
// Exercise 4: Perfect Forwarding Factory
// =============================================================================

template <typename T, typename... Args>
auto make_resource_logged(Args &&...args)
{
    std::cout << "Creating resource with " << sizeof...(args) << " arguments\n";

    // TODO: Implement perfect forwarding
    // Guide: Use std::make_unique with std::forward for each argument
    // Hint: return std::make_unique<T>(std::forward<Args>(args)...);
}

// TODO: Implement a function that detects value categories
// Guide: Create template function that prints whether argument is lvalue or rvalue
template <typename T>
void analyze_value_category(T &&val)
{
    std::cout << "Received: ";

    // TODO: Use if constexpr and type traits to detect lvalue vs rvalue
    // Guide: Check if T is lvalue reference using std::is_lvalue_reference_v<T>
    // Hint: if constexpr (std::is_lvalue_reference_v<T>) { ... } else { ... }

    std::cout << " with value: " << val << std::endl;
}

void perfect_forwarding_test()
{
    std::cout << "\n=== Exercise 4: Perfect Forwarding Test ===\n";

    // TODO: Test the factory with different argument types
    // Guide: Try creating Resource with different combinations of lvalue/rvalue args

    int id = 42;
    std::string name = "test";

    // TODO: Test with lvalue arguments
    // Hint: auto res1 = make_resource_logged<Resource>(id, name);

    // TODO: Test with rvalue arguments
    // Hint: auto res2 = make_resource_logged<Resource>(100, std::string("rvalue"));

    // TODO: Test with mixed arguments
    // Hint: auto res3 = make_resource_logged<Resource>(std::move(id), name);

    // TODO: Test value category analysis
    // Guide: Call analyze_value_category with different types of arguments

    std::string test_str = "lvalue";

    // TODO: Test with lvalue
    // Hint: analyze_value_category(test_str);

    // TODO: Test with rvalue
    // Hint: analyze_value_category(std::move(test_str));

    // TODO: Test with temporary
    // Hint: analyze_value_category(std::string("temporary"));

    std::cout << "Perfect forwarding test completed!\n";
}

// =============================================================================
// Exercise 5: Move-Only Type
// =============================================================================

class MoveOnlyResource
{
private:
    std::unique_ptr<int[]> data_;
    size_t size_;

public:
    explicit MoveOnlyResource(size_t size) : size_(size), data_(new int[size])
    {
        std::fill_n(data_.get(), size_, 42);
        std::cout << "MoveOnlyResource created with size " << size_ << std::endl;
    }

    // TODO: Delete copy constructor and copy assignment
    // Guide: Use = delete to prevent copying
    // Hint: MoveOnlyResource(const MoveOnlyResource&) = delete;

    // TODO: Implement move constructor
    // Guide: Move data_ and size_ from other, mark noexcept
    // Hint: data_(std::move(other.data_)), size_(other.size_)

    // TODO: Implement move assignment
    // Guide: Move data and size, don't forget self-assignment check

    ~MoveOnlyResource()
    {
        std::cout << "MoveOnlyResource destroyed\n";
    }

    size_t size() const { return size_; }
    int *data() const { return data_.get(); }
};

void move_only_test()
{
    std::cout << "\n=== Exercise 5: Move-Only Type Test ===\n";

    // TODO: Create MoveOnlyResource
    MoveOnlyResource res1(100);

    // TODO: Try to move it (should work)
    // Hint: MoveOnlyResource res2 = std::move(res1);

    // TODO: Try to copy it (should not compile - comment out when testing)
    // MoveOnlyResource res3 = res2;  // This should fail!

    // TODO: Create vector of move-only resources
    // Guide: Use emplace_back to construct in place, or push_back with move

    std::vector<MoveOnlyResource> resources;

    // TODO: Add resources to vector using move
    // Hint: resources.emplace_back(50); or resources.push_back(MoveOnlyResource(50));

    std::cout << "Move-only test completed!\n";
}

// =============================================================================
// Main Function - Run All Exercises
// =============================================================================

int main()
{
    std::cout << "🚀 Move Semantics Exercises\n";
    std::cout << "===========================\n";

    try
    {
        // TODO: Uncomment exercises as you complete them

        test_string_wrapper();
        // performance_test();
        // smart_pointer_exercises();
        // perfect_forwarding_test();
        // move_only_test();

        std::cout << "\n🎯 Complete the TODO items above, then uncomment the function calls!\n";
        std::cout << "\n📚 Learning Objectives:\n";
        std::cout << "1. Implement Rule of 5 correctly\n";
        std::cout << "2. Understand performance benefits of move semantics\n";
        std::cout << "3. Master smart pointer usage patterns\n";
        std::cout << "4. Perfect forwarding in templates\n";
        std::cout << "5. Design move-only types\n";

        std::cout << "\n✅ When you complete all exercises, you'll have mastered:\n";
        std::cout << "  - Move constructors and assignment operators\n";
        std::cout << "  - Performance optimization with moves\n";
        std::cout << "  - Smart pointer best practices\n";
        std::cout << "  - Perfect forwarding patterns\n";
        std::cout << "  - Move-only resource management\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
