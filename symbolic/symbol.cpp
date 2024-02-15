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

#include <locale>
#include <jive/strings.h>

#include "settings.h"
#include "symbol.h"
#include "named.h"
#include "value.h"
#include "greek.h"


#include <iostream>


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

    auto s = stringStream.str();

    size_t count = 0;
    size_t index = 0;

    while (index < s.size())
    {
        char c = s[index];

        index += 1;

        if (c & 0x80)
        {
            if (c & 0x40)
            {
                index += 1;

                if (c & 0x20)
                {
                    index += 1;

                    if (c & 0x10)
                    {
                        index += 1;
                    }
                }
            }
        }

        count += 1;
    }

    return count;
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
    if (settings::printCompact)
    {
        switch (op)
        {
            case Op::add:
                return output << "+";

            case Op::subtract:
                return output << "-";

            case Op::multiply:
                return output << "*";

            case Op::divide:
                return output << "/";

            case Op::none:
                return output << "";

            default:
                throw std::logic_error("Invalid operator");
        }
    }
    else
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


std::map<std::string, std::shared_ptr<Arg>> args_;


Arg::Arg(const std::string &name)
    :
    std::string(name)
{

}


void Arg::ClearValue()
{
    this->value_.reset();
}


void Arg::SetValue(double value)
{
    this->value_ = value;
}


std::optional<double> Arg::GetValue() const
{
    return this->value_;
}


std::shared_ptr<Arg> Arg::CreateShared_(const std::string &name)
{
    auto arg = new Arg(name);

    try
    {
        return std::shared_ptr<Arg>(arg);
    }
    catch (...)
    {
        delete arg;
        throw;
    }
}


std::shared_ptr<Arg> Arg::Get(const std::string &name)
{
    if (args_.count(name))
    {
        return args_[name];
    }

    args_[name] = Arg::CreateShared_(name);

    return args_[name];
}


static const std::vector<std::string> trigNames
    {"sin", "cos", "tan", "sec", "csc", "cot"};


static const std::map<std::string, int> trigSortOrder{
    {"sin", 0},
    {"cos", 1},
    {"tan", 2},
    {"sec", 3},
    {"csc", 4},
    {"cot", 5}
};


static const std::map<std::string, std::string> shortTrigNames{
    {"sin", "s"},
    {"cos", "c"},
    {"tan", "t"},
    {"sec", "se"},
    {"csc", "cs"},
    {"cot", "ct"}
};


bool IsTrigName(const std::string &name)
{
    if (name.size() != 3)
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


double GetTrigValue(const std::string &name, double argValue)
{
    // isTrig_
    if (name == "sin")
    {
        return std::sin(argValue);
    }
    else if (name == "cos")
    {
        return std::cos(argValue);
    }
    else if (name == "tan")
    {
        return std::tan(argValue);
    }
    else if (name == "sec")
    {
        return 1.0 / std::cos(argValue);
    }
    else if (name == "csc")
    {
        return 1.0 / std::sin(argValue);
    }
    else if (name == "cot")
    {
        return 1.0 / std::tan(argValue);
    }
    else
    {
        throw std::logic_error("Not a supported trig function");
    }
}


SymbolName::SymbolName(const std::string &name)
    :
    name_{},
    arg_{},
    isTrig_(IsTrigName(name))
{
    if (this->isTrig_)
    {
        this->name_ = name.substr(0, 3);
        auto arg = jive::strings::Trim(name.substr(3));
        this->arg_ = Arg::Get(jive::strings::Trim(arg, "()"));
    }
    else
    {
        this->arg_ = Arg::Get(name);
    }
}


SymbolName::SymbolName(const std::string &name, const std::string &arg)
    :
    name_{},
    arg_{},
    isTrig_(IsTrigName(name))
{
    if (!this->isTrig_)
    {
        throw std::runtime_error(
            "This constructor is only for trig functions.");
    }

    this->name_ = name;
    this->arg_ = Arg::Get(arg);
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
        std::string name;
        if (settings::printCompact)
        {
            name = shortTrigNames.at(this->name_);
        }
        else
        {
            name = this->name_;
        }

        if (*powerValue != 1)
        {
            return output << name << "^" << *powerValue << "(" << *this->arg_ << ")";
        }

        return output << name << "(" << *this->arg_ << ")";
    }

    if (*powerValue != 1)
    {
        return output << *this->arg_ << "^" << *powerValue;
    }

    return output << *this->arg_;
}


std::string SymbolName::GetName() const
{
    std::ostringstream output;
    this->ToStream(output, Value(1));

    return output.str();
}


bool SymbolName::operator==(const SymbolName &other) const
{
    return this->name_ == other.name_ && this->arg_.get() == other.arg_.get();
}


bool SymbolName::operator!=(const SymbolName &other) const
{
    return !this->operator==(other);
}


bool SymbolName::operator<(const SymbolName &other) const
{
    if (this->isTrig_)
    {
        if (greek::IsGreek(*this->arg_) && greek::IsGreek(*other.arg_))
        {
            return greek::sortOrder.at(*this->arg_)
                < greek::sortOrder.at(*other.arg_);
        }
        else
        {
            return *this->arg_ < *other.arg_;
        }
    }
    
    assert(this->name_.empty());

    return *this->arg_ < *other.arg_;
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


S::S(const std::string &name)
{
    this->Base::operator=(std::make_shared<Named>(SymbolName(name)));
}


S::S(const std::string &name, const std::string &arg)
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
            // left operand is a value equal to 0.
            // 0 - right is -right.
            return S(-1) * right;
        }
    }

    if (right->IsValue())
    {
        auto valuePointer = dynamic_cast<Value *>(right.get());
        assert(valuePointer);

        if (valuePointer->operator==(0))
        {
            // right operand is equal to 0.
            // left - 0 is left.
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


S operator^(S left, S right)
{
    return left->MultiplyPower(right);
}
