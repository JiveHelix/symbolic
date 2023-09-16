/**
  * @file operator.cpp
  *
  * @brief Implements the Operator class.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include <iostream>

#include "operator.h"
#include "value.h"
#include "named.h"


using Pointer = typename Symbol::Pointer;
using Collected = typename Operator::Collected;


template<typename Iterator>
void Collect(Collected &collected, Iterator first, Iterator last)
{
    while (first != last)
    {
        collected[*first].push_back(*first);
        ++first;
    }
}


std::vector<Pointer> Operator::Flatten(const Collected &collected)
{
    std::vector<Pointer> result;

    for (const auto & [first, second]: collected)
    {
        result.insert(
            std::end(result),
            std::begin(second),
            std::end(second));
    }

    std::sort(std::begin(result), std::end(result));

    return result;
}


Operator::Operator(
    Op op,
    Pointer left,
    Pointer right)
    :
    scalar_(S(1)),
    op_(op),
    left_(left),
    right_(right)
{

}


Operator::Operator(
    Pointer scalar,
    Op op,
    Pointer left,
    Pointer right)
    :
    scalar_(scalar),
    op_(op),
    left_(left),
    right_(right)
{

}


bool Operator::IsExpression() const
{
    return true;
}


Op Operator::GetOp() const
{
    return this->op_;
}


Pointer Operator::Sum(
    Op op,
    Pointer left,
    Pointer right)
{
    if (left->IsValue())
    {
        auto valuePointer = dynamic_cast<Value *>(left.get());
        assert(valuePointer);

        if (valuePointer->operator==(0))
        {
            if (op == Op::subtract)
            {
                return right * -1;
            }

            return right;
        }
    }

    if (right->IsValue())
    {
        auto valuePointer = dynamic_cast<Value *>(right.get());
        assert(valuePointer);

        if (valuePointer->operator==(0))
        {
            return left;
        }
    }

#if 1
    auto rightScalar = right->GetScalar();
    auto rightScalarAsValue = dynamic_cast<Value *>(rightScalar.get());
    assert(rightScalarAsValue);

    if (rightScalarAsValue->operator<(0))
    {
        right = right * -1;

        rightScalar = right->GetScalar();
        rightScalarAsValue = dynamic_cast<Value *>(rightScalar.get());
        assert(rightScalarAsValue);

        if (op == Op::add)
        {
            // We are adding a negative.
            // Change the operator to subtraction.
            op = Op::subtract;
        }
        else
        {
            assert(op == Op::subtract);
            op = Op::add;
        }
    }
#endif

    return std::make_shared<Operator>(
        op,
        left,
        right);
}


Pointer Operator::Add(Pointer left, Pointer right)
{
    return Sum(Op::add, left, right);
}


Pointer Operator::Subtract(Pointer left, Pointer right)
{
    return Sum(Op::subtract, left, right);
}


Pointer Operator::Multiply(
    Pointer left,
    Pointer right)
{
    auto leftScalar = left->GetScalar();

    if (leftScalar == 0)
    {
        return leftScalar;
    }

    auto rightScalar = right->GetScalar();

    if (rightScalar == 0)
    {
        return rightScalar;
    }

    auto result = std::make_shared<Operator>(
        leftScalar * rightScalar,
        Op::multiply,
        left->ClearScalar(),
        right->ClearScalar());

    return result;
}


Pointer Operator::Divide(
    Pointer left,
    Pointer right)
{
    auto rightScalar = right->GetScalar();

    if (rightScalar == 0)
    {
        throw std::runtime_error("divide by zero");
    }

    auto leftScalar = left->GetScalar();

    if (leftScalar == 0)
    {
        return leftScalar;
    }

#if 1
    return std::make_shared<Operator>(
        leftScalar / rightScalar,
        Op::divide,
        left->ClearScalar(),
        right->ClearScalar());
#else
    return std::make_shared<Operator>(
        Op::divide,
        left,
        right);

#endif
}


std::vector<Pointer> Operator::GetTerms(Op op) const
{
    std::vector<Pointer> terms;

    if (this->op_ != op)
    {
        terms.push_back(
            std::make_shared<Operator>(
                this->scalar_,
                this->op_,
                this->left_,
                this->right_));

        return terms;
    }

    // operators match

    auto left = dynamic_cast<const Operator *>(this->left_.get());

    if (left && left->op_ == op)
    {
        auto leftTerms = left->GetTerms(op);

        terms.insert(
            std::end(terms),
            std::begin(leftTerms),
            std::end(leftTerms));
    }
    else
    {
        terms.push_back(this->left_);
    }

    auto right = dynamic_cast<Operator *>(this->right_.get());

    if (right && right->op_ == op)
    {
        auto rightTerms = right->GetTerms(op);

        terms.insert(
            std::end(terms),
            std::begin(rightTerms),
            std::end(rightTerms));
    }
    else
    {
        terms.push_back(this->right_);
    }

    return terms;
}


Collected Operator::CollectTerms(Op op, Pointer other) const
{
    Collected collected;

    auto terms = this->GetTerms(op);

    Collect(collected, std::begin(terms), std::end(terms));

    Operator *otherOperator = dynamic_cast<Operator *>(other.get());

    if (otherOperator)
    {
        auto otherTerms = otherOperator->GetTerms(op);
        Collect(collected, std::begin(otherTerms), std::end(otherTerms));
    }
    else
    {
        collected[other].push_back(other);
    }

    return collected;
}


Pointer Operator::GetScalar() const
{
    return this->scalar_;
}


Pointer Operator::ClearScalar() const
{
    return std::make_shared<Operator>(
        S(1),
        this->op_,
        this->left_,
        this->right_);
}


Pointer Operator::MultiplyScalar(Pointer scalar) const
{
    return std::make_shared<Operator>(
        this->scalar_ * scalar,
        this->op_,
        this->left_,
        this->right_);
}


Pointer Operator::operator+(Pointer other) const
{
    auto terms = this->CollectTerms(Op::add, other);

    // Collect and add like terms
    std::vector<Pointer> collectedTerms;

    for (auto [first, second]: terms)
    {
        if (second.empty())
        {
            continue;
        }

        auto item = std::begin(second);
        auto result = *item++;

        while (item != std::end(second))
        {
            if (first->IsExpression() || result->IsExpression())
            {
                result = Operator::Add(result, *item++);
            }
            else
            {
                // item is something that can be added to result.
                result = result + *item++;
            }
        }

        collectedTerms.push_back(result);
    }

    assert(!collectedTerms.empty());

    // Combine the groups of like terms using Operator::Add
    if (collectedTerms.size() == 1)
    {
        return collectedTerms.front();
    }

    auto item = std::begin(collectedTerms);

    auto result = Operator::Add(*item++, *item++);

    while (item != std::end(collectedTerms))
    {
        result = Operator::Add(result, *item++);
    }

    return result;
}


Pointer Operator::operator-(Pointer other) const
{
    auto terms = this->CollectTerms(Op::add, other);

    std::vector<Pointer> collectedTerms;

    for (auto [first, second]: terms)
    {
        if (second.empty())
        {
            continue;
        }

        auto item = std::begin(second);
        auto result = *item++;

        while (item != std::end(second))
        {
            if (first->IsExpression() || result->IsExpression())
            {
                result = Operator::Subtract(result, *item++);
            }
            else
            {
                // item is something that can be added to result.
                result = result - *item++;
            }
        }

        collectedTerms.push_back(result);
    }

    assert(!collectedTerms.empty());

    if (collectedTerms.size() == 1)
    {
        return collectedTerms.front();
    }

    auto item = std::begin(collectedTerms);

    auto result = Operator::Subtract(*item++, *item++);

    while (item != std::end(collectedTerms))
    {
        result =
            Operator::Subtract(result, *item++);
    }

    return result;
}


Pointer Operator::operator*(Pointer other) const
{
    auto terms = Flatten(this->CollectTerms(Op::multiply, other));
    auto term = std::begin(terms);
    Pointer result = *term;
    ++term;

    while (term != std::end(terms))
    {
        if ((*term)->IsExpression() || result->IsExpression())
        {
            result = Operator::Multiply(result, *term++);
        }
        else
        {
            result = result * (*term++);
        }
    }

    return result->MultiplyScalar(this->scalar_);
}


Pointer Operator::operator/(Pointer other) const
{
    auto terms = Flatten(this->CollectTerms(Op::multiply, other));

    auto term = std::begin(terms);

    Pointer result = *term;

    ++term;

    while (term != std::end(terms))
    {
        if ((*term)->IsExpression() || result->IsExpression())
        {
            result = Operator::Divide(result, *term++);
        }
        else
        {
            result = result / (*term++);
        }
    }

    return result->MultiplyScalar(this->scalar_);
}


bool Operator::RequiresParentheses(Pointer member) const
{
    if (!member->IsExpression())
    {
        return false;
    }

    auto memberOp = member->GetOp();

    if (this->op_ == Op::add || this->op_ == Op::subtract)
    {
        return (
            memberOp == Op::multiply
            || memberOp == Op::divide);
    }
    // else
    assert(this->op_ == Op::multiply || this->op_ == Op::divide);

    return (
        memberOp == Op::add
        || memberOp == Op::subtract);
}


std::ostream & Operator::ToStream(std::ostream &output) const
{
    auto scalarAsValue = dynamic_cast<const Value *>(this->scalar_.get());
    assert(scalarAsValue);

    bool hasScalar = *scalarAsValue != 1;
    bool leftParens = this->RequiresParentheses(this->left_);
    bool rightParens = this->RequiresParentheses(this->right_);

    if (hasScalar)
    {
        if (*scalarAsValue == -1)
        {
            output << "-(";
        }
        else
        {
            output << this->scalar_ << "(";
        }
    }

    if (leftParens)
    {
        output << "(";
    }

    output << this->left_;

    if (leftParens)
    {
        output << ")";
    }

    output << this->op_;

    if (rightParens)
    {
        output << "(";
    }

    output << this->right_;

    if (rightParens)
    {
        output << ")";
    }

    if (hasScalar)
    {
        output << ")"; 
    }

    return output;
}
