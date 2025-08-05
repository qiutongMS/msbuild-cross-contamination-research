#include <iostream>

// Forward declarations
void firstModuleFunction();
void secondModuleFunction();
void thirdModuleFunction();

int main() {
    std::cout << "=== MSBuild Target Test Project ===" << std::endl;
    std::cout << "Testing compilation with different preprocessor definitions" << std::endl;
    std::cout << std::endl;

    // Call functions from modules compiled with different preprocessor definitions
    firstModuleFunction();
    secondModuleFunction();
    thirdModuleFunction();

    std::cout << std::endl;
    std::cout << "Build completed successfully!" << std::endl;
    
    return 0;
}
