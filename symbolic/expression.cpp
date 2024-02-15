/**
  * @file expression.cpp
  *
  * @brief Implements the Expression class.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include <iostream>
#include <cassert>

#include "expression.h"
#include "value.h"
#include "named.h"


using Pointer = typename Symbol::Pointer;
using Collected = typename Expression::Collected;


std::ostream & operator<<(std::ostream &output, const Collected &collected)
{
    for (const auto &values: collected)
    {
        output << values.front() << '\n';

        for (auto &symbol: values)
        {
            output << "  " << symbol << std::endl;
        }
    }

    return output;
}


template<typename Iterator>
void CollectProduct(Collected &collected, Iterator item)
{
    auto found = std::find_if(
        std::begin(collected),
        std::end(collected),
        [item](const auto &values) -> bool
        {
            return values.front()->PowersAdd(*item)
                || values.front()->SortProduct(*item);
        });

    if (found != std::end(collected))
    {
        found->push_back(*item);
    }
    else
    {
        collected.emplace_back();
        collected.back().push_back(*item);
    }
}


template<typename Iterator>
void CollectProducts(Collected &collected, Iterator first, Iterator last)
{
    while (first != last)
    {
        CollectProduct(collected, first);
        ++first;
    }
}


template<typename Iterator>
void CollectSum(Collected &collected, Iterator item)
{
    auto found = std::find_if(
        std::begin(collected),
        std::end(collected),
        [item](const auto &values) -> bool
        {
            return values.front()->ScalarsAdd(*item);
        });

    if (found != std::end(collected))
    {
        found->push_back(*item);
    }
    else
    {
        collected.emplace_back();
        collected.back().push_back(*item);
    }
}


template<typename Iterator>
void CollectSums(Collected &collected, Iterator first, Iterator last)
{
    while (first != last)
    {
        CollectSum(collected, first);
        ++first;
    }
}


template<typename Iterator>
void CollectItem(Op op, Collected &collected, Iterator item)
{
    if (op == Op::add || op == Op::subtract)
    {
        CollectSum(collected, item);
    }
    else if (op == Op::multiply || op == Op::divide)
    {
        CollectProduct(collected, item);
    }
}


template<typename Iterator>
void CollectItems(Op op, Collected &collected, Iterator first, Iterator last)
{
    if (op == Op::add || op == Op::subtract)
    {
        CollectSums(collected, first, last);
    }
    else if (op == Op::multiply || op == Op::divide)
    {
        CollectProducts(collected, first, last);
    }
}


Expression::Expression(
    Op op,
    Pointer left,
    Pointer right)
    :
    scalar_(S(1)),
    power_(S(1)),
    op_(op),
    left_(left->Copy()),
    right_(right->Copy())
{

}


Expression::Expression(
    Pointer scalar,
    Pointer power,
    Op op,
    Pointer left,
    Pointer right)
    :
    scalar_(scalar->Copy()),
    power_(power->Copy()),
    op_(op),
    left_(left->Copy()),
    right_(right->Copy())
{

}


bool Expression::IsExpression() const
{
    return true;
}


bool Expression::IsTrig() const
{
    return this->left_->IsTrig() && this->right_->IsTrig();
}


Op Expression::GetOp() const
{
    return this->op_;
}


Pointer Expression::Sum(
    Op op,
    Pointer left,
    Pointer right)
{
    if (left->IsZero())
    {
        if (op == Op::subtract)
        {
            return right * -1;
        }

        return right;
    }
    
    if (right->IsZero())
    {
        return left;
    }

    if (left->ScalarsAdd(right))
    {
        auto scalar = left->GetScalar() + right->GetScalar();
        auto result = left->ClearScalar();

        return result->MultiplyScalar(scalar);
    }

    auto rightCopy = right->Copy();

    if (op == Op::subtract)
    {
        op = Op::add;
        rightCopy = right * -1;
    }

    if (left->IsNegative() && rightCopy->IsNegative())
    {
        return std::make_shared<Expression>(
            S(-1),
            S(1),
            op,
            left * -1,
            rightCopy * -1);
    }

    return std::make_shared<Expression>(
        op,
        left,
        rightCopy);
}


Pointer Expression::Add(Pointer left, Pointer right)
{
    return Sum(Op::add, left, right);
}


Pointer Expression::Subtract(Pointer left, Pointer right)
{
    return Sum(Op::subtract, left, right);
}


Pointer Expression::Multiply(
    Pointer left,
    Pointer right)
{
    if (left->IsZero())
    {
        return S(0);
    }

    if (right->IsZero())
    {
        return S(0);
    }

    if (left->IsOne())
    {
        return right;
    }

    if (left->IsNegativeOne())
    {
        return right * -1;
    }

    if (right->IsOne())
    {
        return left;
    }

    if (right->IsNegativeOne())
    {
        return left * -1;
    }

    if (left->PowersAdd(right))
    {
        auto result = left->ClearScalar()->ClearPower();
        result = result->MultiplyScalar(left->GetScalar() * right->GetScalar());

        return result->AddPower(left->GetPower() + right->GetPower() - 1);
    }

    if (left->SortProduct(right))
    {
        if (right < left)
        {
            return std::make_shared<Expression>(
                left->GetScalar() * right->GetScalar(),
                S(1),
                Op::multiply,
                right->ClearScalar(),
                left->ClearScalar());
        }
    }

    return std::make_shared<Expression>(
        left->GetScalar() * right->GetScalar(),
        S(1),
        Op::multiply,
        left->ClearScalar(),
        right->ClearScalar());
}


Pointer Expression::Divide(
    Pointer left,
    Pointer right)
{
    if (right->IsZero())
    {
        throw std::runtime_error("divide by zero");
    }

    if (left->IsZero())
    {
        return S(0);
    }

    auto rightScalar = right->GetScalar();
    auto leftScalar = left->GetScalar();

    if (left->PowersAdd(right))
    {
        auto result = left->ClearScalar()->ClearPower();
        result = result->MultiplyScalar(leftScalar / rightScalar);

        return result->AddPower(left->GetPower() - right->GetPower() - 1);
    }

    return std::make_shared<Expression>(
        leftScalar / rightScalar,
        S(1),
        Op::multiply,
        left->ClearScalar(),
        right->ClearScalar()->Invert());
}


std::vector<Pointer> Expression::GetTerms(Op op) const
{
    std::vector<Pointer> terms;

    if (this->op_ != op)
    {
        terms.push_back(this->Copy());

        return terms;
    }

    // operators match

    auto left = dynamic_cast<const Expression *>(this->left_.get());

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

    auto right = dynamic_cast<Expression *>(this->right_.get());

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


Collected Expression::CollectTerms(Op op, Pointer other) const
{
    Collected collected;

    auto terms = this->GetTerms(op);

    CollectItems(op, collected, std::begin(terms), std::end(terms));

    Expression *otherExpression = dynamic_cast<Expression *>(other.get());

    if (otherExpression)
    {
        auto otherTerms = otherExpression->GetTerms(op);

        CollectItems(
            op,
            collected,
            std::begin(otherTerms),
            std::end(otherTerms));
    }
    else
    {
        CollectItem(op, collected, &other);
    }

    return collected;
}


Pointer Expression::GetScalar() const
{
    return this->scalar_;
}


Pointer Expression::ClearScalar() const
{
    return std::make_shared<Expression>(
        S(1),
        this->power_,
        this->op_,
        this->left_,
        this->right_);
}


Pointer Expression::GetPower() const
{
    return this->power_;
}


Pointer Expression::ClearPower() const
{
    return std::make_shared<Expression>(
        this->scalar_,
        S(1),
        this->op_,
        this->left_,
        this->right_);
}


Pointer Expression::MultiplyScalar(Pointer scalar) const
{
    return std::make_shared<Expression>(
        this->scalar_ * scalar,
        this->power_,
        this->op_,
        this->left_,
        this->right_);
}


Pointer Expression::AddPower(Pointer power) const
{
    return std::make_shared<Expression>(
        this->scalar_,
        this->power_ + power,
        this->op_,
        this->left_,
        this->right_);
}


Pointer Expression::MultiplyPower(Pointer power) const
{
    return std::make_shared<Expression>(
        this->scalar_,
        this->power_ * power,
        this->op_,
        this->left_,
        this->right_);
}


Pointer Expression::operator+(Pointer other) const
{
    if (other->IsZero())
    {
        return this->Copy();
    }

    if (this->IsZero())
    {
        return other;
    }

    auto terms = this->CollectTerms(Op::add, other);

    // Collect and add like terms
    std::vector<Pointer> collectedTerms;

    for (const auto &items: terms)
    {
        if (items.empty())
        {
            continue;
        }

        auto item = std::begin(items);
        auto result = *item++;

        while (item != std::end(items))
        {
            if ((*item)->IsExpression() || result->IsExpression())
            {
                // One or the other is an expression. xor.
                result = Expression::Add(result, *item++);
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

    // Combine the groups of like terms using Expression::Add
    if (collectedTerms.size() == 1)
    {
        return collectedTerms.front();
    }

    // Sort positive terms first.
    std::sort(
        std::begin(collectedTerms),
        std::end(collectedTerms),
        [](const auto &first, const auto &)
        {
            return !first->IsNegative();
        });

    auto item = std::begin(collectedTerms);

    auto result = Expression::Add(*item++, *item++);

    while (item != std::end(collectedTerms))
    {
        result = Expression::Add(result, *item++);
    }

    return result;
}


Pointer Expression::operator-(Pointer other) const
{
    // All subtract operators are turned into add.
    return this->operator+(-1 * other);
}


Pointer Expression::operator*(Pointer other) const
{
    if (other->IsValue())
    {
        return this->MultiplyScalar(other);
    }

    if (this->IsOne())
    {
        return other;
    }

    if (this->IsZero() || other->IsZero())
    {
        return S(0);
    }
         
    if (other->IsOne())
    {
        return this->Copy();
    }

    auto terms = this->CollectTerms(Op::multiply, other);

    // Collect and multiply like terms
    std::vector<Pointer> collectedTerms;

    for (auto &items: terms)
    {
        if (items.empty())
        {
            continue;
        }

        if (items.front()->SortProduct(items.front()))
        {
            std::sort(
                std::begin(items),
                std::end(items));
        }

        auto item = std::begin(items);
        auto result = *item++;

        while (item != std::end(items))
        {
            if ((*item)->IsExpression() || result->IsExpression())
            {
                // One or the other is an expression. xor.
                result = Expression::Multiply(result, *item++);
            }
            else
            {
                // item is something that can be multiplied by result.
                result = result * *item++;
            }
        }

        collectedTerms.push_back(result);
    }

    assert(!collectedTerms.empty());

    // Combine the groups of like terms using Expression::Add
    if (collectedTerms.size() == 1)
    {
        return collectedTerms.front();
    }

    auto item = std::begin(collectedTerms);

    auto result = Expression::Multiply(*item++, *item++);

    while (item != std::end(collectedTerms))
    {
        result = Expression::Multiply(result, *item++);
    }

    return result->MultiplyScalar(this->scalar_);
}


Pointer Expression::operator/(Pointer other) const
{
    return this->operator*(other->Invert());
}


bool Expression::RequiresParentheses(Pointer member) const
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


std::ostream & Expression::ToStream(std::ostream &output) const
{
    if (this->IsZero())
    {
        return output << "0";
    }

    bool hasScalar = !this->scalar_->IsOne();
    bool hasPower = !this->power_->IsOne();
    bool leftParens = this->RequiresParentheses(this->left_);
    bool rightParens = this->RequiresParentheses(this->right_);

    if (hasScalar || hasPower)
    {
        if (this->scalar_->IsNegative())
        {
            output << "-(";
        }
        else if (!this->scalar_->IsOne())
        {
            output << this->scalar_ << "(";
        }
        else
        {
            output << "(";
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

#if 1
    auto rightScalar = this->right_->GetScalar();
    auto rightScalarAsValue = dynamic_cast<Value *>(rightScalar.get());
    assert(rightScalarAsValue);
#endif

    Pointer right = this->right_->Copy();
    Op op = this->op_;

#if 1
    if (op == Op::add && rightScalarAsValue->operator<(0))
    {
        op = Op::subtract;
        right = right * -1;
    }
#endif

    output << op;

    if (rightParens)
    {
        output << "(";
    }

    output << right;

    if (rightParens)
    {
        output << ")";
    }

    if (hasScalar || hasPower)
    {
        output << ")"; 
    }

    if (hasPower)
    {
        output << "^" << this->power_;
    }

    return output;
}


Pointer Expression::Copy() const
{
    return std::make_shared<Expression>(
        this->scalar_->Copy(),
        this->power_->Copy(),
        this->op_,
        this->left_->Copy(),
        this->right_->Copy());
}


Pointer Expression::Invert() const
{
    return std::make_shared<Expression>(
        this->scalar_->Invert(),
        this->power_ * -1,
        this->op_,
        this->left_->Copy(),
        this->right_->Copy());
}


bool Expression::ScalarsAdd(Pointer other) const
{
    if (!this->PowersAdd(other))
    {
        return false;
    }

    if (!this->power_->Equals(other->GetPower()))
    {
        return false;
    }

    return true;
}


bool Expression::PowersAdd(Pointer other) const
{
    auto otherExpression = dynamic_cast<Expression *>(other.get());

    if (otherExpression)
    {
        return this->InsideEquals_(*otherExpression);
    }

    return false;
}


bool Expression::InsideEquals_(const Expression &other) const
{
    return this->op_ == other.op_
        && this->left_->Equals(other.left_)
        && this->right_->Equals(other.right_);
}


bool Expression::Equals(Pointer other) const
{
    auto otherExpression = dynamic_cast<Expression *>(other.get());

    if (!otherExpression)
    {
        return false;
    }

    return this->InsideEquals_(*otherExpression)
        && this->power_->Equals(otherExpression->power_)
        && this->scalar_->Equals(otherExpression->scalar_);
}


bool Expression::IsOne() const
{
    return this->power_->IsZero() && this->scalar_->IsOne();
}


bool Expression::IsNegativeOne() const
{
    return this->power_->IsZero() && this->scalar_->IsNegativeOne();
}


bool Expression::IsZero() const
{
    return this->scalar_->IsZero();
}


bool Expression::IsNegative() const
{
    return this->scalar_->IsNegative();
}
