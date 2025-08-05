#include <iostream>

void thirdModuleFunction() {
    std::cout << "Third Module Function Called:" << std::endl;
    
#ifdef GLOBAL_DEFINE
    std::cout << "  - GLOBAL_DEFINE is defined (value: " << GLOBAL_DEFINE << ")" << std::endl;
#else
    std::cout << "  - GLOBAL_DEFINE is NOT defined" << std::endl;
#endif

#ifdef GLOBAL_ADDITIONAL
    std::cout << "  - GLOBAL_ADDITIONAL is defined (value: " << GLOBAL_ADDITIONAL << ")" << std::endl;
#else
    std::cout << "  - GLOBAL_ADDITIONAL is NOT defined" << std::endl;
#endif

#ifdef MODULE_ONE
    std::cout << "  - MODULE_ONE is defined (value: " << MODULE_ONE << ")" << std::endl;
#else
    std::cout << "  - MODULE_ONE is NOT defined" << std::endl;
#endif

#ifdef FIRST_TARGET
    std::cout << "  - FIRST_TARGET is defined (value: " << FIRST_TARGET << ")" << std::endl;
#else
    std::cout << "  - FIRST_TARGET is NOT defined" << std::endl;
#endif

#ifdef MODULE_TWO
    std::cout << "  - MODULE_TWO is defined (value: " << MODULE_TWO << ")" << std::endl;
#else
    std::cout << "  - MODULE_TWO is NOT defined" << std::endl;
#endif

#ifdef SECOND_TARGET
    std::cout << "  - SECOND_TARGET is defined (value: " << SECOND_TARGET << ")" << std::endl;
#else
    std::cout << "  - SECOND_TARGET is NOT defined" << std::endl;
#endif

#ifdef MODULE_THREE
    std::cout << "  - MODULE_THREE is defined (value: " << MODULE_THREE << ")" << std::endl;
#else
    std::cout << "  - MODULE_THREE is NOT defined" << std::endl;
#endif

#ifdef THIRD_TARGET
    std::cout << "  - THIRD_TARGET is defined (value: " << THIRD_TARGET << ")" << std::endl;
#else
    std::cout << "  - THIRD_TARGET is NOT defined" << std::endl;
#endif

    std::cout << "  - Compiled by: ThirdTarget.targets" << std::endl;
}
