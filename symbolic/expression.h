/**
  * @file expression.h
  *
  * @brief The Expression class represents expressions that cannot be simplified.
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


class Expression: public Symbol
{
public:
    using Symbol::Pointer;

    using Collected = std::vector<std::vector<Pointer>>;

    Expression(Op op, Pointer left, Pointer right);

    Expression(
        Pointer scalar,
        Pointer power,
        Op op,
        Pointer left,
        Pointer right);

    bool IsExpression() const override;

    bool IsTrig() const override;

    Op GetOp() const override;

    Pointer GetLeft()
    {
        return this->left_;
    }

    Pointer GetRight()
    {
        return this->right_;
    }

    static Pointer Sum(Op op, Pointer left, Pointer right);

    static Pointer Add(Pointer left, Pointer right);

    static Pointer Subtract(Pointer left, Pointer right);

    static Pointer Multiply(Pointer left, Pointer right);

    static Pointer Divide(Pointer left, Pointer right);

    std::vector<Pointer> GetTerms(Op op) const;

    Collected CollectTerms(Op op, Pointer other) const;

    Pointer GetScalar() const override;

    Pointer ClearScalar() const override;

    Pointer GetPower() const override;

    Pointer ClearPower() const override;

    Pointer MultiplyScalar(Pointer scalar) const override;

    Pointer AddPower(Pointer power) const override;

    Pointer MultiplyPower(Pointer power) const override;

    Pointer operator+(Pointer other) const override;

    Pointer operator-(Pointer other) const override;

    Pointer operator*(Pointer other) const override;

    Pointer operator/(Pointer other) const override;

    bool RequiresParentheses(Pointer member) const;

    std::ostream & ToStream(std::ostream &output) const override;

    Pointer Copy() const override;

    Pointer Invert() const override;

    bool ScalarsAdd(Pointer other) const override;

    bool PowersAdd(Pointer other) const override;

    bool Equals(Pointer other) const override;

    bool IsOne() const override;

    bool IsZero() const override;

    bool IsNegative() const override;

    bool IsNegativeOne() const override;

protected:
    bool InsideEquals_(const Expression &other) const;

private:
    Pointer scalar_;
    Pointer power_;
    Op op_;
    Pointer left_;
    Pointer right_;
};


std::ostream & operator<<(
    std::ostream &,
    const typename Expression::Collected &);
