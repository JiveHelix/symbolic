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
#include "operator.h"
#include "named.h"


using Pointer = typename Symbol::Pointer;

Value::Value(int value)
    :
    value_(value),
    divisor_(1)
{

}


Value::Value(int value, int divisor)
    :
    value_(value),
    divisor_(divisor)
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
    return this->value_ == other.value_ && this->divisor_ == other.divisor_;
}


bool Value::operator!=(const Value &other) const
{
    return this->value_ != other.value_ || this->divisor_ != other.divisor_;
}


std::ostream & Value::ToStream(std::ostream &output) const
{
    if (this->divisor_ != 1)
    {
        return output <<
            fmt::format("({}/{})", this->value_, this->divisor_);
    }

    return output << this->value_;
}


Pointer Value::GetScalar() const
{
    return std::make_shared<Value>(*this);
}


Pointer Value::ClearScalar() const
{
    return std::make_shared<Value>(1);
}


Pointer Value::MultiplyScalar(Pointer scalar) const
{
    auto asValue = dynamic_cast<const Value *>(scalar.get());

    if (!asValue)
    {
        throw std::runtime_error("argument must be Value *");
    }

    return std::make_shared<Value>(
        this->value_ * asValue->value_,
        this->divisor_ * asValue->divisor_);
}


Pointer Value::operator+(const Value &other) const
{
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

    return std::make_shared<Value>(result, divisor);
}


Pointer Value::operator-(const Value &other) const
{
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

    return std::make_shared<Value>(result, divisor);
}


Pointer Value::operator*(const Value &other) const
{
    return std::make_shared<Value>(
        this->value_ * other.value_,
        this->divisor_ * other.divisor_);
}


Pointer Value::operator/(const Value &other) const
{
    return std::make_shared<Value>(
        this->value_ * other.divisor_,
        this->divisor_ * other.value_);
}


Pointer Value::operator+(Pointer other) const
{
    Value *otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator+(*otherValue);
    }

    return Operator::Add(std::make_shared<Value>(*this), other);
}


Pointer Value::operator-(Pointer other) const
{
    Value *otherValue = dynamic_cast<Value *>(other.get());

    if (otherValue)
    {
        return this->operator-(*otherValue);
    }

    return Operator::Subtract(std::make_shared<Value>(*this), other);
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

    return Operator::Multiply(std::make_shared<Value>(*this), other);
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

    return Operator::Divide(std::make_shared<Value>(*this), other);
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
