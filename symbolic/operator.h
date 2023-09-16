/**
  * @file operator.h
  *
  * @brief The Operator class represents expressions that cannot be simplified.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include <map>
#include "symbol.h"
#include "value.h"


class Operator: public Symbol
{
public:
    using Symbol::Pointer;

    using Collected = std::map<Pointer, std::vector<Pointer>>;

    static std::vector<Pointer> Flatten(const Collected &collected);

    Operator(Op op, Pointer left, Pointer right);

    Operator(Pointer scalar, Op op, Pointer left, Pointer right);

    bool IsExpression() const override;

    Op GetOp() const override;

    static Pointer Sum(Op op, Pointer left, Pointer right);

    static Pointer Add(Pointer left, Pointer right);

    static Pointer Subtract(Pointer left, Pointer right);

    static Pointer Multiply(Pointer left, Pointer right);

    static Pointer Divide(Pointer left, Pointer right);

    std::vector<Pointer> GetTerms(Op op) const;

    Collected CollectTerms(Op op, Pointer other) const;

    Pointer GetScalar() const override;

    Pointer ClearScalar() const override;

    Pointer MultiplyScalar(Pointer scalar) const override;

    Pointer operator+(Pointer other) const override;

    Pointer operator-(Pointer other) const override;

    Pointer operator*(Pointer other) const override;

    Pointer operator/(Pointer other) const override;

    bool RequiresParentheses(Pointer member) const;

    std::ostream & ToStream(std::ostream &output) const override;

private:
    Pointer scalar_;
    Op op_;
    Pointer left_;
    Pointer right_;
};
