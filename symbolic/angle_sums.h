#pragma once


#include <ostream>
#include <fmt/core.h>
#include "symbolic/symbolic.h"


struct SumAndDifference
{
    using Pointer = typename Symbol::Pointer;

    Pointer cosSumName;
    Pointer cosSum;

    Pointer sinSumName;
    Pointer sinSum;

    Pointer cosDifferenceName;
    Pointer cosDifference;

    Pointer sinDifferenceName;
    Pointer sinDifference;

    SumAndDifference(const std::string &first, const std::string &second);
};


std::ostream & operator<<(std::ostream &output, const SumAndDifference &);


Matrix ReplaceAngleSums(const Matrix &matrix);
