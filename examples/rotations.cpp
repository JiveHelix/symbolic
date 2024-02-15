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
#include <symbolic/angle_sums.h>
#include <fmt/core.h>
#include <tau/angles.h>


using namespace greek;


struct NamedMatrix
{
    std::shared_ptr<Arg> arg;
    std::string name;
    std::string axis;
    Matrix matrix;

    NamedMatrix(const std::string &arg_, const std::string &axis_)
        :
        arg(Arg::Get(arg_)),
        name(fmt::format("R({})", arg_)),
        axis(axis_),
        matrix(3, 3)
    {

    }

    void Print() const
    {
        fmt::print(
            "{} (rotation about the {} axis):\n",
            this->name,
            this->axis);

        std::cout << this->matrix << std::endl;
    }
};


void PrintRotation(
    const NamedMatrix &first,
    const NamedMatrix &second,
    const NamedMatrix &third)
{
    auto product = fmt::format(
        " {} * {} * {} ",
        first.name,
        second.name,
        third.name);

    fmt::print("\n{:*^79}\n", product);

    std::cout << first.matrix * second.matrix * third.matrix << std::endl;

    std::cout << "\nSetting " << *second.arg << " to pi/2" << std::endl;
    second.arg->SetValue(tau::Angles<double>::pi / 2);
    auto matrix1 = first.matrix * second.matrix * third.matrix;
    std::cout << matrix1 << std::endl;
    std::cout << "\nUsing angle sum/difference identities" << std::endl;
    std::cout << ReplaceAngleSums(matrix1) << std::endl;

    std::cout << "\nSetting " << *second.arg << " to -pi/2" << std::endl;
    second.arg->SetValue(-tau::Angles<double>::pi / 2);

    auto matrix2 = first.matrix * second.matrix * third.matrix;
    std::cout << matrix2 << std::endl;

    std::cout << "\nUsing angle sum/difference identities" << std::endl;
    std::cout << ReplaceAngleSums(matrix2) << std::endl;

    second.arg->ClearValue();
}


int main()
{
    std::string sin("sin");
    std::string cos("cos");

    NamedMatrix alpha(small::alpha, "x");
    NamedMatrix beta(small::beta, "y");
    NamedMatrix gamma(small::gamma, "z");

    auto sinAlpha = S{sin, small::alpha};
    auto cosAlpha = S{cos, small::alpha};

    alpha.matrix.Assign(
        1, 0, 0,
        0, cosAlpha, -1 * sinAlpha,
        0, sinAlpha, cosAlpha);

    auto sinBeta = S{sin, small::beta};
    auto cosBeta = S{cos, small::beta};

    beta.matrix.Assign(
        cosBeta, 0, sinBeta,
        0, 1, 0,
        -1 * sinBeta, 0, cosBeta);

    auto sinGamma = S{sin, small::gamma};
    auto cosGamma = S{cos, small::gamma};

    gamma.matrix.Assign(
        cosGamma, -1 * sinGamma, 0,
        sinGamma, cosGamma, 0,
        0, 0, 1);

    alpha.Print();
    beta.Print();
    gamma.Print();

    settings::printCompact = true;

    PrintRotation(gamma, beta, alpha);
    PrintRotation(beta, gamma, alpha);
    PrintRotation(gamma, alpha, beta);
    PrintRotation(alpha, beta, gamma);
    PrintRotation(alpha, gamma, beta);
    PrintRotation(beta, gamma, alpha);
    PrintRotation(beta, alpha, gamma);

    return 0;
}
