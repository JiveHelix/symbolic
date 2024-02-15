/**
  * @file greek.h
  *
  * @brief Letters of the Greek alphabet as UTF-8.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include <ostream>
#include <map>


namespace greek
{


namespace small
{
    constexpr auto alpha = "\u03B1";
    constexpr auto beta = "\u03B2";
    constexpr auto gamma = "\u03B3";
    constexpr auto delta = "\u03B4";
    constexpr auto epsilon = "\u03B5";
    constexpr auto zeta = "\u03B6";
    constexpr auto eta = "\u03B7";
    constexpr auto theta = "\u03B8";
    constexpr auto iota = "\u03B9";
    constexpr auto kappa = "\u03BA";
    constexpr auto lambda = "\u03BB";
    constexpr auto mu = "\u03BC";
    constexpr auto nu = "\u03BD";
    constexpr auto xi = "\u03BE";
    constexpr auto omicron = "\u03BF";
    constexpr auto pi = "\u03C0";
    constexpr auto rho = "\u03C1";
    constexpr auto sigmaf = "\u03C2";
    constexpr auto sigma = "\u03C3";
    constexpr auto tau = "\u03C4";
    constexpr auto upsilon = "\u03C5";
    constexpr auto phi = "\u03C6";
    constexpr auto chi = "\u03C7";
    constexpr auto psi = "\u03C8";
    constexpr auto omega = "\u03C9";
}


namespace capital
{
    constexpr auto alpha = "\u0391";
    constexpr auto beta = "\u0392";
    constexpr auto gamma = "\u0393";
    constexpr auto delta = "\u0394";
    constexpr auto epsilon = "\u0395";
    constexpr auto zeta = "\u0396";
    constexpr auto eta = "\u0397";
    constexpr auto theta = "\u0398";
    constexpr auto iota = "\u0399";
    constexpr auto kappa = "\u039A";
    constexpr auto lambda = "\u039B";
    constexpr auto mu = "\u039C";
    constexpr auto nu = "\u039D";
    constexpr auto xi = "\u039E";
    constexpr auto omicron = "\u039F";
    constexpr auto pi = "\u03A0";
    constexpr auto rho = "\u03A1";
    constexpr auto sigma = "\u03A3";
    constexpr auto tau = "\u03A4";
    constexpr auto upsilon = "\u03A6";
    constexpr auto phi = "\u03A6";
    constexpr auto chi = "\u03A7";
    constexpr auto psi = "\u03A8";
    constexpr auto omega = "\u03A9";
}


std::ostream & Display(std::ostream &);


extern const std::map<std::string, int> sortOrder;


bool IsGreek(const std::string &value);


} // end namespace greek
