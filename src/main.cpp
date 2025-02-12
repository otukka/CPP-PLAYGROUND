
#include <iostream>
#include <limits>

void run();

int main()
{
    std::cout << "Program started.\n";

    int a = std::numeric_limits<int>::min();
    int b = std::numeric_limits<int>::max();


    std::cout << a << std::endl;
    std::cout << b << std::endl;
    
    std::cout << std::hex << a << std::endl;
    std::cout << std::hex << b << std::endl;

    std::cout << std::hex << static_cast<unsigned int>(a) << std::endl;
    std::cout << std::hex << static_cast<unsigned int>(b) << std::endl;



    std::cout << "Program ended.\n";

    return 0;
}
