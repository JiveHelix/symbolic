/**
  * @file value.h
  *
  * @brief Represents exact rational values.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include "symbol.h"


class Value: public Symbol
{
public:
    using Symbol::Pointer;

    Value(int value);

    Value(int value, int divisor);

    bool IsValue() const override;

    static int GreatestCommonDivisor(int left, int right);

    bool operator==(const Value &other) const;

    bool operator!=(const Value &other) const;

    std::ostream & ToStream(std::ostream &output) const override;

    Pointer GetScalar() const override;

    Pointer ClearScalar() const override;

    Pointer MultiplyScalar(Pointer scalar) const override;

    Pointer operator+(const Value &other) const;

    Pointer operator-(const Value &other) const;

    Pointer operator*(const Value &other) const;

    Pointer operator/(const Value &other) const;

    Pointer operator+(Pointer other) const override;

    Pointer operator-(Pointer other) const override;

    Pointer operator*(Pointer other) const override;

    Pointer operator/(Pointer other) const override;

    bool operator<(const Value &other) const;

private:
    int value_;
    int divisor_;
};
