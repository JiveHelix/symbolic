#pragma once


#include <ostream>
#include <fmt/core.h>
#include "symbolic/symbolic.h"


struct Identity
{
    using Pointer = typename Symbol::Pointer;

    Pointer name;
    Pointer expression;
};


struct SumAndDifference
{
    Identity cosSum;
    Identity sinSum;
    Identity cosDifference;
    Identity sinDifference;

    SumAndDifference(const std::string &first, const std::string &second);
};


std::ostream & operator<<(std::ostream &output, const SumAndDifference &);


Matrix ReplaceAngleSums(const Matrix &matrix);
