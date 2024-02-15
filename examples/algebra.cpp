/**
  * @file rotations.cpp
  *
  * @brief An algebra demonstration.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 12 Feb 2024
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include <iostream>
#include <symbolic/symbolic.h>
#include <symbolic/greek.h>
#include <fmt/core.h>


int main()
{
    auto x = S("x");
    auto y = S("y");
    auto z = S("z");

    auto p = (x + 2);
    auto q = (x - 7);

    std::cout << "p: " << p << std::endl;
    std::cout << "q: " << q << std::endl;

    std::cout << "p * q: " << p * q << std::endl;
    std::cout << "p * p: " << p * p << std::endl;

    std::cout << "p * q * p: " << p * q * p << std::endl;

    std::cout << "p^3: " << (p^3) << std::endl;
    std::cout << "p^3 / p: " << (p^3) / p << std::endl;
    std::cout << "p / p^3: " << p / (p^3) << std::endl;

    // std::cout << 2 * x + 3 * y - 2 * z + 5 * x - 1 * y + x << std::endl;
    // std::cout << 2 * x - 2 * z - z << std::endl;
    // std::cout << z * x * y * x / z << std::endl;

    return 0;
}
