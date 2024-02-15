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

    Value(int value, int divisor, int powerValue, int powerDivisor);

    bool IsValue() const override;

    static int GreatestCommonDivisor(int left, int right);

    bool operator==(const Value &other) const;

    std::ostream & ToStream(std::ostream &output) const override;

    Pointer GetScalar() const override;

    Pointer ClearScalar() const override;

    Pointer GetPower() const override;

    Pointer ClearPower() const override;

    Pointer MultiplyScalar(Pointer scalar) const override;

    Pointer AddPower(Pointer power) const override;

    Pointer MultiplyPower(Pointer power) const override;

    Pointer operator+(const Value &other) const;

    Pointer operator-(const Value &other) const;

    Pointer operator*(const Value &other) const;

    Pointer operator/(const Value &other) const;

    Pointer operator+(Pointer other) const override;

    Pointer operator-(Pointer other) const override;

    Pointer operator*(Pointer other) const override;

    Pointer operator/(Pointer other) const override;

    bool operator<(const Value &other) const;
    bool operator>(const Value &other) const;

    Pointer Copy() const override;

    Pointer Invert() const override;

    bool ScalarsAdd(Pointer other) const override;

    bool PowersAdd(Pointer other) const override;

    bool Equals(Pointer other) const override;

    bool HasPower() const;

    bool IsOne() const override;

    bool IsNegativeOne() const override;

    bool IsZero() const override;

    bool IsNegative() const override;

    template<typename T>
    T GetValue() const
    {
        auto result = std::pow(
            static_cast<double>(this->value_)
                / static_cast<double>(this->divisor_),
            static_cast<double>(this->powerValue_)
                / static_cast<double>(this->powerDivisor_));

        if constexpr (std::is_integral_v<T>)
        {
            return static_cast<T>(std::round(result));
        }
        else
        {
            return static_cast<T>(result);
        }
    }

    bool IsIntegral() const
    {
        return this->divisor_ == 1;
    }

    int GetIntegral() const
    {
        return this->value_;
    }

protected:
    Value GetPower_() const;

private:
    int value_;
    int divisor_;
    int powerValue_;
    int powerDivisor_;
};
