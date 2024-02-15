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

#include <cassert>


using Pointer = typename Symbol::Pointer;


Named::Named(const SymbolName &name)
    :
    name_(name),
    scalar_(1),
    power_(1)
{

}


Named::Named(
    const SymbolName &name,
    Pointer value,
    Pointer power)
    :
    name_(name),
    scalar_(1),
    power_(1)
{
    auto valuePointer = dynamic_cast<Value *>(value.get());

    if (!valuePointer)
    {
        throw std::runtime_error("Expected a Value");
    }

    this->scalar_ = *valuePointer;

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
    scalar_(value),
    power_(power)
{

}


Named::Named(
    const SymbolName &name,
    Pointer value,
    const Value &power)
    :
    name_(name),
    scalar_(1),
    power_(power)
{
    auto valuePointer = dynamic_cast<Value *>(value.get());

    if (!valuePointer)
    {
        throw std::runtime_error("Expected a Value");
    }

    this->scalar_ = *valuePointer;
}


bool Named::IsNamed() const
{
    return true;
}


bool Named::IsTrig() const
{
    return this->name_.IsTrig();
}


#if 0
bool Named::operator==(const Named &other) const
{
    if (other.name_ != this->name_)
    {
        return false;
    }

    return this->scalar_ == other.scalar_ && this->power_ == other.power_;
}


bool Named::operator!=(const Named &other) const
{
    return !this->operator==(other);
}
#endif


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
    return std::make_shared<Value>(this->scalar_);
}


Pointer Named::ClearScalar() const
{
    return std::make_shared<Named>(this->name_, S(1), this->power_);
}


Pointer Named::GetPower() const
{
    return std::make_shared<Value>(this->power_);
}


Pointer Named::ClearPower() const
{
    return std::make_shared<Named>(this->name_, this->scalar_, Value(1));
}


Pointer Named::MultiplyScalar(Pointer scalar) const
{
    return std::make_shared<Named>(
        this->name_,
        this->scalar_ * scalar,
        this->power_);
}


Pointer Named::AddPower(Pointer power) const
{
    auto powerValue = dynamic_cast<Value *>(power.get());

    if (!powerValue)
    {
        throw std::runtime_error("Power must be a value");
    }

    return std::make_shared<Named>(
        this->name_,
        this->scalar_.Copy(),
        this->power_ + *powerValue);
}


Pointer Named::MultiplyPower(Pointer power) const
{
    auto powerValue = dynamic_cast<Value *>(power.get());

    if (!powerValue)
    {
        throw std::runtime_error("Power must be a value");
    }

    return std::make_shared<Named>(
        this->name_,
        this->scalar_.Copy(),
        this->power_ * *powerValue);
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
            auto valueResult = this->scalar_ + otherNamed->scalar_;

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

    return Expression::Add(std::make_shared<Named>(*this), other);
}


Pointer Named::operator-(Pointer other) const
{
    auto otherNamed = dynamic_cast<Named *>(other.get());

    if (otherNamed)
    {
        if (otherNamed->name_ == this->name_
                && otherNamed->power_ == this->power_)
        {
            auto valueResult = this->scalar_ - otherNamed->scalar_;

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

    return Expression::Subtract(std::make_shared<Named>(*this), other);
}


Pointer Named::operator*(const Value &other) const
{
    auto valueResult = this->scalar_ * other;
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
        this->scalar_ / other,
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
                return std::make_shared<Value>(this->scalar_);
            }

            return std::make_shared<Named>(
                this->name_,
                this->scalar_ * otherNamed->scalar_,
                resultPower);
        }
    }

    auto otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator*(*otherValue);
    }

    return Expression::Multiply(std::make_shared<Named>(*this), other);
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
                return std::make_shared<Value>(this->scalar_);
            }

            return std::make_shared<Named>(
                this->name_,
                this->scalar_ * otherNamed->scalar_,
                resultPower);
        }
    }

    auto otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator/(*otherValue);
    }

    return Expression::Divide(std::make_shared<Named>(*this), other);
}


std::ostream & Named::ToStream(std::ostream &output) const
{
    auto value = this->GetValue<int>();

    if (value)
    {
        return output << *value;
    }

    if (this->power_ == 0)
    {
        return output << this->scalar_;
    }

    if (this->scalar_ == -1)
    {
        output << "-";
    }
    else if (this->scalar_ != 1)
    {
        output << this->scalar_ << "*";
    }

    this->name_.ToStream(output, this->power_);

    return output;
}


Pointer Named::Copy() const
{
    return std::make_shared<Named>(this->name_, this->scalar_, this->power_);
}


Pointer Named::Invert() const
{
    return std::make_shared<Named>(
        this->name_,
        this->scalar_.Invert(),
        this->power_ * -1);
}


bool Named::ScalarsAdd(Pointer other) const
{
    auto otherNamed = dynamic_cast<Named *>(other.get());

    if (!otherNamed)
    {
        return false;
    }

    return this->name_ == otherNamed->name_
            && this->power_ == otherNamed->power_;
}


bool Named::PowersAdd(Pointer other) const
{
    auto otherNamed = dynamic_cast<Named *>(other.get());

    if (!otherNamed)
    {
        return false;
    }

    return this->name_ == otherNamed->name_;
}


bool Named::Equals(Pointer other) const
{
    auto otherNamed = dynamic_cast<Named *>(other.get());

    if (!otherNamed)
    {
        return false;
    }

    return this->name_ == otherNamed->name_
            && this->power_ == otherNamed->power_
            && this->scalar_ == otherNamed->scalar_;
}


bool Named::IsOne() const
{
    if (this->power_ == 0 && this->scalar_ == 1)
    {
        return true;
    }

    auto value = this->name_.GetValue<double>();

    if (value)
    {
        return std::abs(*value - 1.0) < 0.001;
    }
    
    return false;
}


bool Named::IsNegativeOne() const
{
    if (this->power_ == 0 && this->scalar_ == -1)
    {
        return true;
    }

    auto value = this->name_.GetValue<double>();

    if (value)
    {
        return std::abs(*value + 1.0) < 0.001;
    }
    
    return false;
}


bool Named::IsZero() const
{
    auto value = this->name_.GetValue<double>();

    if (value)
    {
        return std::abs(*value) < 0.001;

    }

    return (this->scalar_ == 0);
}


bool Named::IsNegative() const
{
    auto value = this->name_.GetValue<double>();

    if (!value)
    {
        // Without a specific value, the sign of this term is just the sign of
        // the scalar.
        return this->scalar_ < 0;
    }

    if (!this->power_.IsIntegral())
    {
        // Cannot say definitively whether result will be negative (or
        // imaginary).
        return false;
    }

    if (this->power_.GetIntegral() % 2 == 0)
    {
        // Even power. Any value will be positive.
        // scalar_ determines the sign.
        return this->scalar_ < 0;
    }

    // Odd power.
    if (*value < 0)
    {
        // value will still be negative after exponentiation.
        return this->scalar_ > 0;
    }

    // value is positive. Odd power is still positive.
    return this->scalar_ < 0;
}


bool Named::SortProduct(Pointer other) const
{
    auto otherNamed = dynamic_cast<Named *>(other.get());

    if (!otherNamed)
    {
        return false;
    }

    return this->name_.IsTrig() && otherNamed->name_.IsTrig();
}
