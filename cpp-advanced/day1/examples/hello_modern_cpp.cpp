#include <iostream>
#include <vector>
#include <format>
#include <ranges>
#include <algorithm>

int main() {
    std::cout << "=== Modern C++ Test ===\n";
    
    // Test C++20 format
    std::cout << std::format("C++ version: {}\n", __cplusplus);
    
    // Test ranges
    std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    auto even_numbers = numbers 
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; });
    
    std::cout << "Even squares: ";
    for (int n : even_numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n";
    
    return 0;
}
