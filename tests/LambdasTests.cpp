
#include <catch2/catch_all.hpp>
#include <iostream>
#include "Ringbuffer.h"

namespace
{

std::function<void()> get_lambda()
{
    int variable{0};
    // std::cout << "Variable address: " << &variable << "\n";
    const auto lambda = [variable]() mutable
    {
        std::cout << "Variable address: " << &variable << " Variable value: " << variable++ << std::endl;
    };
    return lambda;
}

void lambda_print_copy(std::function<void()> lambda)
{
    std::cout << "Lambda address: " << &lambda << " ";
    lambda();
}
void lambda_print_reference(const std::function<void()>& lambda)
{
    std::cout << "Lambda address: " << &lambda << " ";
    lambda();
}
void lambda_print_move(std::function<void()>&& lambda)
{
    std::cout << "Lambda address: " << &lambda << " ";
    lambda();
}

std::function<void()> lambda_wrapper_copy_copy(std::function<void()> lambda)
{
    const auto wrapper = [lambda]()
    {
        std::cout << "lambda_wrapper_copy_copy: " << &lambda << " ";
        lambda();
    };

    return wrapper;
}
std::function<void()> lambda_wrapper_copy_reference(const std::function<void()>& lambda)
{
    const auto wrapper = [lambda]()
    {
        std::cout << "lambda_wrapper_copy_reference: " << &lambda << " ";
        lambda();
    };

    return wrapper;
}
std::function<void()> lambda_wrapper_copy_move(std::function<void()>&& lambda)
{
    const auto wrapper = [lambda]()
    {
        std::cout << "lambda_wrapper_copy_move: " << &lambda << " ";
        lambda();
    };

    return wrapper;
}

std::function<void()> lambda_wrapper_reference_copy(std::function<void()> lambda)
{
    const auto wrapper = [&lambda]()
    {
        std::cout << "lambda_wrapper_reference_copy: " << &lambda << " ";
        lambda();
    };

    return wrapper;
}

std::function<void()> lambda_wrapper_reference_reference(const std::function<void()>& lambda)
{
    const auto wrapper = [&lambda]()
    {
        std::cout << "lambda_wrapper_reference_reference: " << &lambda << " ";
        lambda();
    };

    return wrapper;
}

std::function<void()> lambda_wrapper_reference_move(std::function<void()>&& lambda)
{
    const auto wrapper = [&lambda]()
    {
        std::cout << "lambda_wrapper_reference_move: " << &lambda << " ";
        lambda();
    };

    return wrapper;
}

void recursive_lambda_copy(std::function<void()> lambda, int count)
{
    if (count < 5)
    {
        std::cout << "recursive_lambda_copy: " << &lambda << " ";
        lambda();
        recursive_lambda_copy(lambda, ++count);
    }
}

void recursive_lambda_reference(const std::function<void()>& lambda, int count)
{
    if (count < 5)
    {
        std::cout << "recursive_lambda_reference: " << &lambda << " ";
        lambda();
        recursive_lambda_reference(lambda, ++count);
    }
}

void recursive_lambda_move(std::function<void()>&& lambda, int count)
{
    if (count < 5)
    {
        std::cout << "recursive_lambda_move: " << &lambda << " ";
        lambda();
        recursive_lambda_move(std::move(lambda), ++count);
    }
}
}

TEST_CASE("lambda_bare")
{
    // The lambda is created inside get_lambda(), and each time it is called, a new lambda object is created.
    // The address of the lambda should be the same within the function, but a new lambda will be created each time
    // get_lambda() is called.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    for (size_t i = 0; i < 5; i++)
    {
        lambda();
    }
}

TEST_CASE("lambda_bare wrong")
{
    // A new lambda is created on each iteration, so the lambda address should be different on every call to
    // get_lambda().
    for (size_t i = 0; i < 5; i++)
    {
        get_lambda()();
    }
}

TEST_CASE("lambda_print")
{

    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";

    // The lambda is passed by value (copied) into lambda_print_copy(),
    // so the address of the lambda inside lambda_print_copy will be different on each call.
    lambda_print_copy(lambda);

    // The lambda is passed by reference to lambda_print_reference(),
    // so the lambda address should remain the same across all invocations of the function.

    lambda_print_reference(lambda);

    // The lambda is moved into lambda_print_move(), so ownership changes during each call.
    // However, the lambda address should stay consistent unless an internal move occurs.
    lambda_print_move(std::move(lambda));
}

TEST_CASE("lambda_wrapper_copy_copy")
{
    // The lambda is copied into lambda_wrapper_copy_copy(), creating a new copy.
    // Therefore, the lambda address inside the wrapper will be different than the original lambda.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    const auto wrapped = lambda_wrapper_copy_copy(lambda);
    for (size_t i = 0; i < 5; i++)
    {
        wrapped();
    }
}

TEST_CASE("lambda_wrapper_copy_reference")
{
    // The lambda is passed by reference into lambda_wrapper_copy_reference(),
    // so the address of the lambda inside the wrapper will remain the same as the original lambda.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    const auto wrapped = lambda_wrapper_copy_reference(lambda);
    for (size_t i = 0; i < 5; i++)
    {
        wrapped();
    }
}

TEST_CASE("lambda_wrapper_copy_move")
{
    // The lambda is moved into lambda_wrapper_copy_move(),
    // so the address inside the wrapper should remain the same as the original lambda before it was moved.
    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    const auto wrapped = lambda_wrapper_copy_move(std::move(lambda));
    for (size_t i = 0; i < 5; i++)
    {
        wrapped();
    }
}

TEST_CASE("lambda_wrapper_reference_copy")
{
    // The lambda is captured by reference inside lambda_wrapper_reference_copy(),
    // so the address of the lambda should remain the same within the wrapper.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    const auto wrapped = lambda_wrapper_reference_copy(lambda);
    for (size_t i = 0; i < 5; i++)
    {
        wrapped();
    }
}

TEST_CASE("lambda_wrapper_reference_reference")
{
    // The lambda is passed by reference into lambda_wrapper_reference_reference(),
    // so the address of the lambda inside the wrapper should remain the same as the original.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    const auto wrapped = lambda_wrapper_reference_reference(lambda);
    for (size_t i = 0; i < 5; i++)
    {
        wrapped();
    }
}

TEST_CASE("lambda_wrapper_reference_move")
{
    // The lambda is moved into lambda_wrapper_reference_move(),
    // so the lambda address inside the wrapper should be the same as the original before it was moved.
    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    const auto wrapped = lambda_wrapper_reference_move(std::move(lambda));
    for (size_t i = 0; i < 5; i++)
    {
        wrapped();
    }
}

TEST_CASE("recursive_lambda_copy")
{
    // The lambda is passed by copy in each recursive step,
    // so a new copy with a new address will be created during each recursive call.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_copy(lambda, 0);
}

TEST_CASE("recursive_lambda_reference")
{
    // The lambda is passed by reference in each recursive step,
    // so the address of the lambda should remain the same across all recursive calls.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_reference(lambda, 0);
}

TEST_CASE("recursive_lambda_move")
{
    // The lambda is passed by move in each recursive step,
    // so ownership will transfer, but the lambda address should remain consistent within the function.
    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_move(std::move(lambda), 0);
}

TEST_CASE("recursive_lambda_copy_lambda_wrapper_copy_copy")
{
    // The lambda is passed by copy in both the lambda wrapper and recursive call.
    // A new copy is created during each recursion step, resulting in different addresses.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_copy(lambda_wrapper_copy_copy(lambda), 0);
}

TEST_CASE("recursive_lambda_reference_lambda_wrapper_copy_copy")
{
    // The lambda is passed by copy in the wrapper and by reference in the recursive function,
    // so a new address will be created in the wrapper, but recursion keeps the same lambda reference.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_reference(lambda_wrapper_copy_copy(lambda), 0);
}

TEST_CASE("recursive_lambda_move_lambda_wrapper_copy_copy")
{
    // The lambda is passed by copy in the wrapper and by move in the recursive function.
    // Each step in recursion keeps the same lambda reference.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_move(lambda_wrapper_copy_copy(lambda), 0);
}

TEST_CASE("recursive_lambda_copy_lambda_wrapper_copy_reference")
{
    // The lambda is passed by reference in both the wrapper and recursive call,
    // so the lambda address remains the same throughout.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_copy(lambda_wrapper_copy_reference(lambda), 0);
}

TEST_CASE("recursive_lambda_reference_lambda_wrapper_copy_reference")
{
    // Both the wrapper and recursive function pass the lambda by reference,
    // so the lambda address should stay the same across all calls.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_reference(lambda_wrapper_copy_reference(lambda), 0);
}

TEST_CASE("recursive_lambda_move_lambda_wrapper_copy_reference")
{
    // The lambda is passed by reference in the wrapper and by move in the recursive function.
    // Ownership is transferred during recursion, but the address should remain the same.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_move(lambda_wrapper_copy_reference(lambda), 0);
}

TEST_CASE("recursive_lambda_copy_lambda_wrapper_copy_move")
{
    // The lambda is passed by move in the wrapper and copy in the recursive function.
    // The lambda is moved into the wrapper, but a new copy is created during recursion, resulting in different
    // addresses.
    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_copy(lambda_wrapper_copy_move(std::move(lambda)), 0);
}

TEST_CASE("recursive_lambda_reference_lambda_wrapper_copy_move")
{
    // The lambda is passed by move in the wrapper and by reference in the recursive function.
    // Ownership is transferred to the wrapper, but recursion keeps the same lambda reference.
    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_reference(lambda_wrapper_copy_move(std::move(lambda)), 0);
}

TEST_CASE("recursive_lambda_move_lambda_wrapper_copy_move")
{
    // The lambda is passed by move in both the wrapper and recursive function.
    // Ownership transfers across both steps, but the lambda address should remain consistent.
    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_move(lambda_wrapper_copy_move(std::move(lambda)), 0);
}

TEST_CASE("recursive_lambda_copy_lambda_wrapper_reference_copy")
{
    // The lambda is passed by reference in the wrapper and by copy in the recursive function.
    // The lambda address should remain consistent in the wrapper, but a new copy is made during recursion.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_copy(lambda_wrapper_reference_copy(lambda), 0);
}

TEST_CASE("recursive_lambda_reference_lambda_wrapper_reference_copy")
{
    // The lambda is passed by reference in both the wrapper and the recursive function.
    // The lambda address should remain the same throughout the wrapper and recursion steps.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_reference(lambda_wrapper_reference_copy(lambda), 0);
}

TEST_CASE("recursive_lambda_move_lambda_wrapper_reference_copy")
{
    // The lambda is passed by reference in the wrapper and by move in the recursive function.
    // The lambda address remains the same in the wrapper, and ownership is transferred during recursion.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_move(lambda_wrapper_reference_copy(lambda), 0);
}

TEST_CASE("recursive_lambda_copy_lambda_wrapper_reference_reference")
{
    // The lambda is passed by reference in both the wrapper and the recursive function.
    // The lambda address should remain the same throughout both the wrapper and the recursive calls.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_copy(lambda_wrapper_reference_reference(lambda), 0);
}

TEST_CASE("recursive_lambda_reference_lambda_wrapper_reference_reference")
{
    // The lambda is passed by reference in both the wrapper and the recursive function.
    // The lambda address remains consistent throughout both the wrapper and the recursive calls.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_reference(lambda_wrapper_reference_reference(lambda), 0);
}

TEST_CASE("recursive_lambda_move_lambda_wrapper_reference_reference")
{
    // The lambda is passed by reference in the wrapper and by move in the recursive function.
    // Ownership is transferred during recursion, but the lambda address remains the same in the wrapper.
    const auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_move(lambda_wrapper_reference_reference(lambda), 0);
}

TEST_CASE("recursive_lambda_copy_lambda_wrapper_reference_move")
{
    // The lambda is moved into the wrapper and then passed by copy during recursion.
    // The lambda address in the wrapper should remain consistent, but a new copy is made during recursion.
    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_copy(lambda_wrapper_reference_move(std::move(lambda)), 0);
}

TEST_CASE("recursive_lambda_reference_lambda_wrapper_reference_move")
{
    // The lambda is moved into the wrapper and passed by reference during recursion.
    // The lambda address remains consistent throughout both the wrapper and recursion steps.
    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_reference(lambda_wrapper_reference_move(std::move(lambda)), 0);
}

TEST_CASE("recursive_lambda_move_lambda_wrapper_reference_move")
{
    // The lambda is moved into both the wrapper and the recursive function.
    // Ownership is transferred in both cases, but the lambda address should remain consistent throughout.
    auto lambda = get_lambda();
    std::cout << "Lambda address: " << &lambda << "\n";
    recursive_lambda_move(lambda_wrapper_reference_move(std::move(lambda)), 0);
}