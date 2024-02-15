/**
  * @file named.h
  *
  * @brief A class for named symbols.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include <string>
#include "symbolic/symbol.h"
#include "symbolic/expression.h"
#include "symbolic/value.h"


class Named: public Symbol
{
public:
    using Symbol::Pointer;

    Named(const SymbolName &name);

    Named(const SymbolName &name, Pointer value, Pointer power);

    Named(const SymbolName &name, const Value &value, const Value &power);

    Named(const SymbolName &name, Pointer value, const Value &power);

    bool IsNamed() const override;

    bool IsTrig() const override;

    bool operator<(const Named &other) const;

    Pointer GetScalar() const override;

    Pointer ClearScalar() const override;

    Pointer GetPower() const override;

    Pointer ClearPower() const override;

    Pointer MultiplyScalar(Pointer scalar) const override;

    Pointer AddPower(Pointer power) const override;

    Pointer MultiplyPower(Pointer power) const override;

    Pointer operator+(Pointer other) const override;

    Pointer operator-(Pointer other) const override;

    Pointer operator*(const Value &other) const;

    Pointer operator/(const Value &other) const;

    Pointer operator*(Pointer other) const override;

    Pointer operator/(Pointer other) const override;

    std::ostream & ToStream(std::ostream &output) const override;

    Pointer Copy() const override;

    Pointer Invert() const override;

    bool ScalarsAdd(Pointer other) const override;

    bool PowersAdd(Pointer other) const override;

    bool Equals(Pointer other) const override;

    bool IsOne() const override;

    bool IsNegativeOne() const override;

    bool IsZero() const override;

    bool IsNegative() const override;

    bool SortProduct(Pointer other) const override;

    template<typename T>
    std::optional<T> GetValue() const
    {
        auto value = this->name_.GetValue<double>();

        if (!value)
        {
            return {};
        }

        auto result =
            this->scalar_.GetValue<double>()
            * std::pow(*value, this->power_.GetValue<double>());

        if constexpr (std::is_integral_v<T>)
        {
            return static_cast<T>(std::round(result));
        }
        else
        {
            return static_cast<T>(result);
        }
    }

    std::shared_ptr<Arg> GetArg()
    {
        return this->name_.GetArg();
    }

private:
    SymbolName name_;
    Value scalar_;
    Value power_;
};
