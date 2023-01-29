
#include <iostream>

void run();

namespace
{
enum asdf
{
    A = 1,
    B = 2,
    C = 3
} qwerty;
}

int main()
{
    std::cout << "Program started.\n";

    for (int i = 10; i >= 0; i--)
    {
        std::cout << i << std::endl;
    }
    
    std::cout << "asdf.\n";

    // run();

    asdf a = A;
    asdf b = qwerty;
    
    std::cout << b << "\n";

    std::cout << a << "\n";
    std::cout << A << "\n";

    std::cout << "Program ended.\n";

    return 0;
}
