#include <iostream>

// Forward declarations
void firstModuleFunction();
void secondModuleFunction();

int main() {
    std::cout << "=== MSBuild Target Test Project ===" << std::endl;
    std::cout << "Testing compilation with different preprocessor definitions" << std::endl;
    std::cout << std::endl;

    // Call functions from modules compiled with different preprocessor definitions
    firstModuleFunction();
    secondModuleFunction();

    std::cout << std::endl;
    std::cout << "Build completed successfully!" << std::endl;
    
    // Keep console window open
    system("pause");
    return 0;
}
