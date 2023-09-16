/**
  * @file named.cpp
  *
  * @brief Implements named symbol operations.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include "named.h"

#include <map>


using Pointer = typename Symbol::Pointer;


Named::Named(const SymbolName &name)
    :
    name_(name),
    value_(1),
    power_(1)
{

}


Named::Named(
    const SymbolName &name,
    Pointer value,
    Pointer power)
    :
    name_(name),
    value_(1),
    power_(1)
{
    auto valuePointer = dynamic_cast<Value *>(value.get());

    if (!valuePointer)
    {
        throw std::runtime_error("Expected a Value");
    }

    this->value_ = *valuePointer;

    auto powerPointer = dynamic_cast<Value *>(power.get());

    if (!powerPointer)
    {
        throw std::runtime_error("Expected a Value");
    }

    this->power_ = *powerPointer;
}


Named::Named(
    const SymbolName &name,
    const Value &value,
    const Value &power)
    :
    name_(name),
    value_(value),
    power_(power)
{

}


Named::Named(
    const SymbolName &name,
    Pointer value,
    const Value &power)
    :
    name_(name),
    value_(1),
    power_(power)
{
    auto valuePointer = dynamic_cast<Value *>(value.get());

    if (!valuePointer)
    {
        throw std::runtime_error("Expected a Value");
    }

    this->value_ = *valuePointer;
}


bool Named::IsNamed() const
{
    return true;
}


bool Named::operator==(const Named &other) const
{
    if (other.name_ != this->name_)
    {
        return false;
    }

    return this->value_ == other.value_ && this->power_ == other.power_;
}


bool Named::operator!=(const Named &other) const
{
    return !this->operator==(other);
}


bool Named::operator<(const Named &other) const
{
    if (this->name_ == other.name_)
    {
        return this->power_ < other.power_;
    }

    return this->name_ < other.name_;
}


Pointer Named::GetScalar() const
{
    return std::make_shared<Value>(this->value_);
}


Pointer Named::ClearScalar() const
{
    return std::make_shared<Named>(this->name_, S(1), this->power_);
}


Pointer Named::MultiplyScalar(Pointer scalar) const
{
    return std::make_shared<Named>(
        this->name_,
        scalar * std::make_shared<Value>(this->value_),
        this->power_);
}


Pointer Named::operator+(Pointer other) const
{
    auto otherNamed = dynamic_cast<Named *>(other.get());

    if (otherNamed)
    {
        if (otherNamed->name_ == this->name_
                && otherNamed->power_ == this->power_)
        {
            // This is the same symbol and can be added.
            auto valueResult = this->value_ + otherNamed->value_;

            if (valueResult == 0)
            {
                return std::make_shared<Value>(0);
            }

            return std::make_shared<Named>(
                this->name_,
                valueResult,
                this->power_);
        }
    }

    return Operator::Add(std::make_shared<Named>(*this), other);
}


Pointer Named::operator-(Pointer other) const
{
    auto otherNamed = dynamic_cast<Named *>(other.get());

    if (otherNamed)
    {
        if (otherNamed->name_ == this->name_
                && otherNamed->power_ == this->power_)
        {
            auto valueResult = this->value_ - otherNamed->value_;

            if (valueResult == 0)
            {
                return std::make_shared<Value>(0);
            }

            return std::make_shared<Named>(
                this->name_,
                valueResult,
                this->power_);
        }
    }

    return Operator::Subtract(std::make_shared<Named>(*this), other);
}


Pointer Named::operator*(const Value &other) const
{
    auto valueResult = this->value_ * other;
    auto asValue = dynamic_cast<Value *>(valueResult.get());

    assert(asValue);

    if (*asValue == 0)
    {
        return std::make_shared<Value>(0);
    }

    return std::make_shared<Named>(
        this->name_,
        valueResult,
        this->power_);
}


Pointer Named::operator/(const Value &other) const
{
    if (other == 0)
    {
        throw std::runtime_error("divide by zero");
    }

    return std::make_shared<Named>(
        this->name_,
        this->value_ / other,
        this->power_);
}


Pointer Named::operator*(Pointer other) const
{
    auto otherNamed = dynamic_cast<Named *>(other.get());

    if (otherNamed)
    {
        if (otherNamed->name_ == this->name_)
        {
            auto resultPower = this->power_ + otherNamed->power_;

            if (resultPower == 0)
            {
                return std::make_shared<Value>(this->value_);
            }

            return std::make_shared<Named>(
                this->name_,
                this->value_ * otherNamed->value_,
                resultPower);
        }
    }

    auto otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator*(*otherValue);
    }

    return Operator::Multiply(std::make_shared<Named>(*this), other);
}


Pointer Named::operator/(Pointer other) const
{
    auto otherNamed = dynamic_cast<Named *>(other.get());

    if (otherNamed)
    {
        if (otherNamed->name_ == this->name_)
        {
            auto resultPower = this->power_ - otherNamed->power_;

            if (resultPower == 0)
            {
                return std::make_shared<Value>(this->value_);
            }

            return std::make_shared<Named>(
                this->name_,
                this->value_ * otherNamed->value_,
                resultPower);
        }
    }

    auto otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator/(*otherValue);
    }

    return Operator::Divide(std::make_shared<Named>(*this), other);
}


std::ostream & Named::ToStream(std::ostream &output) const
{
    if (this->power_ == 0)
    {
        return output << this->value_;
    }

    if (this->value_ == -1)
    {
        output << "-";
    }
    else if (this->value_ != 1)
    {
        output << this->value_ << "*";
    }

    this->name_.ToStream(output, this->power_);

    return output;
}
