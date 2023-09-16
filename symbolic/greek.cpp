/**
  * @file greek.cpp
  *
  * @brief Prints the Greek alphabet.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include "greek.h"


#include <fmt/core.h>


namespace greek
{


std::ostream & Display(std::ostream &output)
{
    output << "Small Greek Alphabet:\n";

    output << fmt::format("{:>20}: {}\n", "alpha",   small::alpha);
    output << fmt::format("{:>20}: {}\n", "beta",    small::beta);
    output << fmt::format("{:>20}: {}\n", "gamma",   small::gamma);
    output << fmt::format("{:>20}: {}\n", "delta",   small::delta);
    output << fmt::format("{:>20}: {}\n", "epsilon", small::epsilon);
    output << fmt::format("{:>20}: {}\n", "zeta",    small::zeta);
    output << fmt::format("{:>20}: {}\n", "eta",     small::eta);
    output << fmt::format("{:>20}: {}\n", "theta",   small::theta);
    output << fmt::format("{:>20}: {}\n", "iota",    small::iota);
    output << fmt::format("{:>20}: {}\n", "kappa",   small::kappa);
    output << fmt::format("{:>20}: {}\n", "lambda",  small::lambda);
    output << fmt::format("{:>20}: {}\n", "mu",      small::mu);
    output << fmt::format("{:>20}: {}\n", "nu",      small::nu);
    output << fmt::format("{:>20}: {}\n", "xi",      small::xi);
    output << fmt::format("{:>20}: {}\n", "omicron", small::omicron);
    output << fmt::format("{:>20}: {}\n", "pi",      small::pi);
    output << fmt::format("{:>20}: {}\n", "rho",     small::rho);
    output << fmt::format("{:>20}: {}\n", "sigmaf",  small::sigmaf);
    output << fmt::format("{:>20}: {}\n", "sigma",   small::sigma);
    output << fmt::format("{:>20}: {}\n", "tau",     small::tau);
    output << fmt::format("{:>20}: {}\n", "upsilon", small::upsilon);
    output << fmt::format("{:>20}: {}\n", "phi",     small::phi);
    output << fmt::format("{:>20}: {}\n", "chi",     small::chi);
    output << fmt::format("{:>20}: {}\n", "psi",     small::psi);
    output << fmt::format("{:>20}: {}\n", "omega",   small::omega);

    output << "\nCapital Greek Alphabet:\n";

    output << fmt::format("{:>20}: {}\n", "alpha",   capital::alpha);
    output << fmt::format("{:>20}: {}\n", "beta",    capital::beta);
    output << fmt::format("{:>20}: {}\n", "gamma",   capital::gamma);
    output << fmt::format("{:>20}: {}\n", "delta",   capital::delta);
    output << fmt::format("{:>20}: {}\n", "epsilon", capital::epsilon);
    output << fmt::format("{:>20}: {}\n", "zeta",    capital::zeta);
    output << fmt::format("{:>20}: {}\n", "eta",     capital::eta);
    output << fmt::format("{:>20}: {}\n", "theta",   capital::theta);
    output << fmt::format("{:>20}: {}\n", "iota",    capital::iota);
    output << fmt::format("{:>20}: {}\n", "kappa",   capital::kappa);
    output << fmt::format("{:>20}: {}\n", "lambda",  capital::lambda);
    output << fmt::format("{:>20}: {}\n", "mu",      capital::mu);
    output << fmt::format("{:>20}: {}\n", "nu",      capital::nu);
    output << fmt::format("{:>20}: {}\n", "xi",      capital::xi);
    output << fmt::format("{:>20}: {}\n", "omicron", capital::omicron);
    output << fmt::format("{:>20}: {}\n", "pi",      capital::pi);
    output << fmt::format("{:>20}: {}\n", "rho",     capital::rho);
    output << fmt::format("{:>20}: {}\n", "sigma",   capital::sigma);
    output << fmt::format("{:>20}: {}\n", "tau",     capital::tau);
    output << fmt::format("{:>20}: {}\n", "upsilon", capital::upsilon);
    output << fmt::format("{:>20}: {}\n", "phi",     capital::phi);
    output << fmt::format("{:>20}: {}\n", "chi",     capital::chi);
    output << fmt::format("{:>20}: {}\n", "psi",     capital::psi);
    output << fmt::format("{:>20}: {}\n", "omega",   capital::omega);

    return output;
}


} // end namespace greek
