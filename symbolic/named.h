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
#include "symbol.h"
#include "operator.h"
#include "value.h"


class Named: public Symbol
{
public:
    using Symbol::Pointer;

    Named(const SymbolName &name);

    Named(const SymbolName &name, Pointer value, Pointer power);

    Named(const SymbolName &name, const Value &value, const Value &power);

    Named(const SymbolName &name, Pointer value, const Value &power);

    bool IsNamed() const override;

    bool operator==(const Named &other) const;

    bool operator!=(const Named &other) const;

    bool operator<(const Named &other) const;

    Pointer GetScalar() const override;

    Pointer ClearScalar() const override;

    Pointer MultiplyScalar(Pointer scalar) const override;

    Pointer operator+(Pointer other) const override;

    Pointer operator-(Pointer other) const override;

    Pointer operator*(const Value &other) const;

    Pointer operator/(const Value &other) const;

    Pointer operator*(Pointer other) const override;

    Pointer operator/(Pointer other) const override;

    std::ostream & ToStream(std::ostream &output) const override;

private:
    SymbolName name_;
    Value value_;
    Value power_;
};


