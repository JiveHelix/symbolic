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
#include <symbolic/symbolic.h>
#include <symbolic/greek.h>
#include <fmt/core.h>


using namespace greek;


int main()
{
    Matrix alphaMatrix(3, 3);
    Matrix betaMatrix(3, 3);
    Matrix gammaMatrix(3, 3);

    auto sinAlpha = S{"sin", small::alpha};
    auto cosAlpha = S{"cos", small::alpha};

    alphaMatrix.Assign(
        1, 0, 0,
        0, cosAlpha, -1 * sinAlpha,
        0, sinAlpha, cosAlpha);

    auto sinBeta = S{"sin", small::beta};
    auto cosBeta = S{"cos", small::beta};

    betaMatrix.Assign(
        cosBeta, 0, sinBeta,
        0, 1, 0,
        -1 * sinBeta, 0, cosBeta);

    auto sinGamma = S{"sin", small::gamma};
    auto cosGamma = S{"cos", small::gamma};

    gammaMatrix.Assign(
        cosGamma, -1 * sinGamma, 0,
        sinGamma, cosGamma, 0,
        0, 0, 1);

    auto alphaMatrixName = fmt::format("R({})", small::alpha);
    auto betaMatrixName = fmt::format("R({})", small::beta);
    auto gammaMatrixName = fmt::format("R({})", small::gamma);

    fmt::print(
        "{} (rotation about the x axis):\n",
        alphaMatrixName);

    std::cout << alphaMatrix << std::endl;

    fmt::print(
        "{} (rotation about the y axis):\n",
        betaMatrixName);

    std::cout << betaMatrix << std::endl;

    fmt::print(
        "{} (rotation about the z axis):\n",
        gammaMatrixName);

    std::cout << gammaMatrix << std::endl;

    fmt::print(
        "\n{} * {} * {}:\n",
        gammaMatrixName,
        betaMatrixName,
        alphaMatrixName);

    std::cout << gammaMatrix * betaMatrix * alphaMatrix << std::endl;

    fmt::print(
        "\n{} * {} * {}:\n",
        alphaMatrixName,
        betaMatrixName,
        gammaMatrixName);

    std::cout << alphaMatrix * betaMatrix * gammaMatrix << std::endl;

    fmt::print(
        "\n{} * {} * {}:\n",
        betaMatrixName,
        gammaMatrixName,
        alphaMatrixName);

    std::cout << betaMatrix * gammaMatrix * alphaMatrix << std::endl;

    return 0;
}
