#include <catch2/catch_all.hpp>
#include <cstdint>   // uint32_t
#include <iostream>  // uint32_t
#include <tuple>     // uint32_t

template<class... contents>
class MyClass
{
private:
public:
    std::tuple<contents...> items;
    MyClass() = default;
    ~MyClass() = default;
};

struct onion
{
    int c;
};

// TEST_CASE("Assingment with MyClass")
// {
//     {
//         MyClass<onion> a{};
//         CHECK(true);
//     }
// }
