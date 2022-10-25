
#include <iostream>

#include <string>
#include <string_view>

static uint32_t allocated = 0;

// redefine memory allocation
void* operator new(size_t size)
{
    std::cout << "Allocating memory on the heap\n";
    allocated++;
    return malloc(size);
}

void printString(const char* str)
{
    std::cout << "const char * : ";
    std::cout << str << std::endl;
}

void printString(const std::string& str)
{
    std::cout << "const string & : ";
    std::cout << str << std::endl;
}

void printString(std::string_view str)
{
    std::cout << "string_view : ";
    std::cout << str << std::endl;
}

int run(int argc, char const* argv[])
{
    const char* string1 = "1234567890123456";
    printString(string1);

    std::string string2 = "1234567890123456";
    printString(string2);

    // Small string optimization
    printString(string2.substr(0, string2.length()));

    // No malloc called in string view
    printString(std::string_view(string2));
    printString(std::string_view(string1, 5));

    std::cout << "Allocated number of times: " << allocated << std::endl;
    return 0;
}
