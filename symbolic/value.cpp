/**
  * @file value.cpp
  *
  * @brief Implements the Value class.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include "value.h"

#include <fmt/core.h>
#include "expression.h"
#include "named.h"


using Pointer = typename Symbol::Pointer;

Value::Value(int value)
    :
    value_(value),
    divisor_(1),
    powerValue_(1),
    powerDivisor_(1)
{

}


Value::Value(int value, int divisor)
    :
    value_(value),
    divisor_(divisor),
    powerValue_(1),
    powerDivisor_(1)
{
    if (divisor == 0)
    {
        throw std::runtime_error("Divide by zero");
    }

    if (divisor < 0)
    {
        this->value_ *= -1;
        this->divisor_ *= -1;
    }

    if (this->value_ == 0)
    {
        this->divisor_ = 1;
    }
    else if (this->divisor_ != 1)
    {
        int greatestCommonDivisor =
            GreatestCommonDivisor(this->value_, this->divisor_);

        if (greatestCommonDivisor != 1)
        {
            this->value_ /= greatestCommonDivisor;
            this->divisor_ /= greatestCommonDivisor;
        }
    }
}


bool Value::IsValue() const
{
    return true;
}


int Value::GreatestCommonDivisor(int left, int right)
{
    if (right == 0)
    {
        return left;
    }

    return GreatestCommonDivisor(right, left % right);
}


bool Value::operator==(const Value &other) const
{
    return 
        this->value_ == other.value_
        && this->divisor_ == other.divisor_
        && this->powerValue_ == other.powerValue_
        && this->powerDivisor_ == other.powerDivisor_;
}


std::ostream & Value::ToStream(std::ostream &output) const
{
    if (this->divisor_ != 1)
    {
        output <<
            fmt::format("({}/{})", this->value_, this->divisor_);
    }
    else
    {
        output << this->value_;
    }

    auto power = this->GetPower_();

    if (power != 1)
    {
        output << '^';
        power.ToStream(output);
    }

    return output;
}


Pointer Value::GetScalar() const
{
    return std::make_shared<Value>(*this);
}


Pointer Value::ClearScalar() const
{
    return std::make_shared<Value>(1);
}


Pointer Value::GetPower() const
{
    return std::make_shared<Value>(this->powerValue_, this->powerDivisor_);
}


Pointer Value::ClearPower() const
{
    return std::make_shared<Value>(this->value_, this->divisor_);
}


bool Value::HasPower() const
{
    return this->powerValue_ != 1 || this->powerDivisor_ != 1;
}


Pointer Value::MultiplyScalar(Pointer scalar) const
{
    auto asValue = dynamic_cast<const Value *>(scalar.get());

    if (!asValue)
    {
        throw std::runtime_error("argument must be Value *");
    }

    if (this->HasPower() || asValue->HasPower())
    {
        throw std::runtime_error("Cannot add values with different powers.");
    }

    return std::make_shared<Value>(
        this->value_ * asValue->value_,
        this->divisor_ * asValue->divisor_);
}


Pointer Value::AddPower(Pointer power) const
{
    auto asValue = dynamic_cast<const Value *>(power.get());

    if (!asValue)
    {
        throw std::runtime_error("argument must be Value *");
    }

    if (asValue->HasPower())
    {
        throw std::runtime_error("Unexpected exponent on power value.");
    }

    auto thisPower = this->GetPower_();

    auto resultPowerValue = thisPower.value_ + asValue->value_;
    auto resultPowerDivisor = thisPower.divisor_ + asValue->divisor_;

    return std::make_shared<Value>(
        this->value_,
        this->divisor_,
        resultPowerValue,
        resultPowerDivisor);
}


Pointer Value::MultiplyPower(Pointer power) const
{
    auto asValue = dynamic_cast<const Value *>(power.get());

    if (!asValue)
    {
        throw std::runtime_error("argument must be Value *");
    }

    if (asValue->HasPower())
    {
        throw std::runtime_error("Unexpected exponent on power value.");
    }

    auto thisPower = this->GetPower_();

    auto resultPowerValue = thisPower.value_ * asValue->value_;
    auto resultPowerDivisor = thisPower.divisor_ * asValue->divisor_;

    return std::make_shared<Value>(
        this->value_,
        this->divisor_,
        resultPowerValue,
        resultPowerDivisor);
}


Pointer Value::operator+(const Value &other) const
{
    if (this->GetPower_() != other.GetPower_())
    {
        throw std::runtime_error("only like-powers can be added.");
    }

    if (this->divisor_ == other.divisor_)
    {
        return std::make_shared<Value>(
            this->value_ + other.value_,
            this->divisor_);
    }

    int left = this->value_ * other.divisor_;
    int right = other.value_ * this->divisor_;
    int result = left + right;
    int divisor = this->divisor_ * other.divisor_;

    return std::make_shared<Value>(
        result,
        divisor,
        this->powerValue_,
        this->powerDivisor_);
}


Pointer Value::operator-(const Value &other) const
{
    if (this->GetPower_() != other.GetPower_())
    {
        throw std::runtime_error("only like-powers can be subtracted.");
    }

    if (this->divisor_ == other.divisor_)
    {
        return std::make_shared<Value>(
            this->value_ - other.value_,
            this->divisor_);
    }

    int left = this->value_ * other.divisor_;
    int right = other.value_ * this->divisor_;
    int result = left - right;
    int divisor = this->divisor_ * other.divisor_;

    return std::make_shared<Value>(
        result,
        divisor,
        this->powerValue_,
        this->powerDivisor_);
}


Pointer Value::operator*(const Value &other) const
{
    if (this->GetPower_() != other.GetPower_())
    {
        throw std::runtime_error("only like-powers can be multiplied.");
    }

    return std::make_shared<Value>(
        this->value_ * other.value_,
        this->divisor_ * other.divisor_,
        this->powerValue_,
        this->powerDivisor_);
}


Pointer Value::operator/(const Value &other) const
{
    if (this->GetPower_() != other.GetPower_())
    {
        throw std::runtime_error("only like-powers can be divided.");
    }

    return std::make_shared<Value>(
        this->value_ * other.divisor_,
        this->divisor_ * other.value_,
        this->powerValue_,
        this->powerDivisor_);
}


Pointer Value::operator+(Pointer other) const
{
    Value *otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator+(*otherValue);
    }

    return Expression::Add(std::make_shared<Value>(*this), other);
}


Pointer Value::operator-(Pointer other) const
{
    Value *otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator-(*otherValue);
    }

    return Expression::Subtract(std::make_shared<Value>(*this), other);
}


Pointer Value::operator*(Pointer other) const
{
    Value *otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator*(*otherValue);
    }

    Named *otherNamed = dynamic_cast<Named *>(other.get());

    if (otherNamed)
    {
        return otherNamed->operator*(*this);
    }

    return Expression::Multiply(std::make_shared<Value>(*this), other);
}


Pointer Value::operator/(Pointer other) const
{
    Value *otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator/(*otherValue);
    }

    Named *otherNamed = dynamic_cast<Named *>(other.get());

    if (otherNamed)
    {
        return otherNamed->operator/(*this);
    }

    return Expression::Divide(std::make_shared<Value>(*this), other);
}


bool Value::operator<(const Value &other) const
{
    if (this->divisor_ == other.divisor_)
    {
        return this->value_ < other.value_;
    }

    int left = this->value_ * other.divisor_;
    int right = other.value_ * this->divisor_;

    return left < right;
}


bool Value::operator>(const Value &other) const
{
    return !this->operator<(other) && !this->operator==(other);
}


Pointer Value::Copy() const
{
    return std::make_shared<Value>(this->value_, this->divisor_);
}


Pointer Value::Invert() const
{
    if (this-value_ == 0)
    {
        throw std::runtime_error("Divide by zero");
    }

    return std::make_shared<Value>(this->divisor_, this->value_);
}


bool Value::ScalarsAdd(Pointer other) const
{
    Value *otherValue = dynamic_cast<Value *>(other.get());

    if (!otherValue)
    {
        return false;
    }

    return true;
}


bool Value::PowersAdd(Pointer other) const
{
    Value *otherValue = dynamic_cast<Value *>(other.get());

    if (!otherValue)
    {
        return false;
    }

    return this->ClearPower() == otherValue->ClearPower();
}


bool Value::Equals(Pointer other) const
{
    Value *otherValue = dynamic_cast<Value *>(other.get());

    if (!otherValue)
    {
        return false;
    }

    return this->operator==(*otherValue);
}


Value Value::GetPower_() const
{
    return Value(this->powerValue_, this->powerDivisor_);
}


Value::Value(int value, int divisor, int powerValue, int powerDivisor)
    :
    value_(value),
    divisor_(divisor),
    powerValue_(powerValue),
    powerDivisor_(powerDivisor)
{

}


bool Value::IsOne() const
{
    return this->value_ == 1 && this->divisor_ == 1;
}


bool Value::IsNegativeOne() const
{
    return this->value_ == -1 && this->divisor_ == 1;
}


bool Value::IsZero() const
{
    return this->value_ == 0;
}


bool Value::IsNegative() const
{
    return this->operator<(0);
}
