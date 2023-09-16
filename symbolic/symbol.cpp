/**
  * @file symbol.cpp
  *
  * @brief Default implementations of Symbol functions.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include <jive/strings.h>

#include "symbol.h"
#include "named.h"
#include "value.h"


Symbol::~Symbol()
{

}


Op Symbol::GetOp() const
{
    return Op::none;
}


bool Symbol::IsExpression() const
{
    return false;
}


bool Symbol::IsValue() const
{
    return false;
}


bool Symbol::IsNamed() const
{
    return false;
}


size_t Symbol::GetDisplayWidth() const
{
    std::ostringstream stringStream;
    this->ToStream(stringStream);
    return stringStream.str().size();
}


std::ostream & operator<<(
    std::ostream &output,
    typename Symbol::Pointer symbol)
{
    return symbol->ToStream(output);
}


std::ostream & operator<<(
    std::ostream &output,
    const Symbol &symbol)
{
    return symbol.ToStream(output);
}


typename Symbol::Pointer operator+(
    typename Symbol::Pointer left,
    typename Symbol::Pointer right)
{
    return left->operator+(right);
}

typename Symbol::Pointer operator-(
    typename Symbol::Pointer left,
    typename Symbol::Pointer right)
{
    return left->operator-(right);
}

typename Symbol::Pointer operator*(
    typename Symbol::Pointer left,
    typename Symbol::Pointer right)
{
    return left->operator*(right);
}

typename Symbol::Pointer operator/(
    typename Symbol::Pointer left,
    typename Symbol::Pointer right)
{
    return left->operator/(right);
}


std::ostream & operator<<(std::ostream &output, const Op &op)
{
    switch (op)
    {
        case Op::add:
            return output << " + ";

        case Op::subtract:
            return output << " - ";

        case Op::multiply:
            return output << " * ";

        case Op::divide:
            return output << " / ";

        case Op::none:
            return output << " ";

        default:
            throw std::logic_error("Invalid operator");
    }
}


bool operator<(typename Symbol::Pointer left, typename Symbol::Pointer right)
{
    if (right->IsExpression())
    {
        // Everything comes before expressions
        return true;
    }

    if (left->IsExpression())
    {
        return false;
    }

    if (left->IsValue())
    {
        // Value always sorts first.
        return true;
    }

    // left isn't value
    if (right->IsValue())
    {
        // but right is, so it must be sorted first.
        return false;
    }

    assert(left->IsNamed());
    assert(right->IsNamed());

    Named *leftNamed = dynamic_cast<Named *>(left.get());
    Named *rightNamed = dynamic_cast<Named *>(right.get());

    assert(leftNamed);
    assert(rightNamed);

    return (*leftNamed < *rightNamed);
}


static const std::vector<std::string> trigNames
    {"sin", "cos", "tan", "sec", "csc", "cot"};


static const std::map<std::string, int> trigSortOrder{
    {"sin", 0},
    {"cos", 1},
    {"tan", 2},
    {"sec", 3},
    {"csc", 4},
    {"cot", 5}};


bool IsTrigName(const std::string &name)
{

    if (name.size() < 3)
    {
        return false;
    }

    auto found =
        std::find(
            std::begin(trigNames),
            std::end(trigNames),
            name.substr(0, 3));

    return (found != std::end(trigNames));
}


SymbolName::SymbolName(const std::string &name)
    :
    name_{name},
    arg_{},
    isTrig_(IsTrigName(name))
{
    if (this->isTrig_)
    {
        this->name_ = name.substr(0, 3);
        auto arg = jive::strings::Trim(name.substr(3));
        this->arg_ = jive::strings::Trim(arg, "()");
    }
}


SymbolName::SymbolName(const std::string &name, const std::string &arg)
    :
    name_{name},
    arg_{arg},
    isTrig_(IsTrigName(name))
{
    if (this->isTrig_)
    {
        this->name_ = name.substr(0, 3);
    }
}


std::ostream & SymbolName::ToStream(
    std::ostream &output,
    const Symbol &power) const
{
    auto powerValue = dynamic_cast<const Value *>(&power);

    assert(powerValue);

    if (*powerValue == 0)
    {
        return output << "1";
    }

    if (this->isTrig_)
    {
        if (*powerValue != 1)
        {
            return output << this->name_ << "^" << *powerValue << "(" << this->arg_ << ")";
        }

        return output << this->name_ << "(" << this->arg_ << ")";
    }

    if (*powerValue != 1)
    {
        return output << this->name_ << "^" << *powerValue;
    }

    return output << this->name_;
}


std::string SymbolName::GetName() const
{
    std::ostringstream output;
    this->ToStream(output, Value(1));

    return output.str();
}


bool SymbolName::operator==(const SymbolName &other) const
{
    return this->name_ == other.name_ && this->arg_ == other.arg_;
}


bool SymbolName::operator!=(const SymbolName &other) const
{
    return !this->operator==(other);
}


bool SymbolName::operator<(const SymbolName &other) const
{
    if (this->name_ == other.name_)
    {
        return this->arg_ < other.arg_;
    }

    if (this->isTrig_)
    {
        return trigSortOrder.at(this->name_) < trigSortOrder.at(other.name_);
    }

    return this->name_ < other.name_;
}


std::ostream & operator<<(std::ostream &output, const SymbolName &symbolName)
{
    return symbolName.ToStream(output, Value(1));
}



S::S(): Base(nullptr)
{

}


S::S(const std::shared_ptr<Symbol> &pointer)
    :
    Base(pointer)
{

}


S::S(int value)
{
    this->Base::operator=(std::make_shared<Value>(value));
}


S::S(const char *name)
{
    this->Base::operator=(std::make_shared<Named>(SymbolName(name)));
}


S::S(const char *name, const char *arg)
{
    this->Base::operator=(std::make_shared<Named>(SymbolName(name, arg)));
}


S operator+(S left, S right)
{
    if (left->IsValue())
    {
        auto valuePointer = dynamic_cast<Value *>(left.get());
        assert(valuePointer);

        if (valuePointer->operator==(0))
        {
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

    return left->operator+(right);
}


S operator-(S left, S right)
{
    if (left->IsValue())
    {
        auto valuePointer = dynamic_cast<Value *>(left.get());
        assert(valuePointer);

        if (valuePointer->operator==(0))
        {
            return S(-1) * right;
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

    return left->operator-(right);
}


S operator*(S left, S right)
{
    return left->operator*(right);
}


S operator/(S left, S right)
{
    return left->operator/(right);
}


