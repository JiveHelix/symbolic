/**
  * @file rotations.cpp
  *
  * @brief A demonstration of intrinsics matrix rotations.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include <iostream>
#include <symbolic/angle_sums.h>


using namespace greek;



int main()
{
    SumAndDifference alphaBeta(small::alpha, small::beta);
    SumAndDifference betaGamma(small::beta, small::gamma);

    std::cout << alphaBeta << std::endl;
    std::cout << betaGamma << std::endl;

    return 0;
}
