/**
  * @file symbol.h
  *
  * @brief The Symbol class defines the interface used by this library.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include <ostream>
#include <sstream>
#include <memory>
#include <iostream>

enum class Op
{
    add,
    subtract,
    multiply,
    divide,
    none
};


template<Op op, typename Enable = void>
struct IsValidOperator: std::false_type {};

template<Op op>
struct IsValidOperator
<
    op,
    std::enable_if_t
    <(
        op == Op::add
        || op == Op::subtract
        || op == Op::multiply
        || op == Op::divide)
    >
>: std::true_type {};


class Symbol
{
public:
    using Pointer = std::shared_ptr<Symbol>;

    virtual ~Symbol();

    virtual std::ostream & ToStream(std::ostream &output) const = 0;

    std::ostream & ToStreamCompact(std::ostream &output) const;

    virtual Op GetOp() const;

    virtual Pointer operator+(Pointer other) const = 0;
    virtual Pointer operator-(Pointer other) const = 0;
    virtual Pointer operator*(Pointer other) const = 0;
    virtual Pointer operator/(Pointer other) const = 0;

    virtual Pointer GetScalar() const = 0;
    virtual Pointer ClearScalar() const = 0;
    virtual Pointer GetPower() const = 0;
    virtual Pointer ClearPower() const = 0;
    virtual Pointer MultiplyScalar(Pointer scalar) const = 0;
    virtual Pointer AddPower(Pointer power) const = 0;
    virtual Pointer MultiplyPower(Pointer power) const = 0;
    virtual bool IsOne() const = 0;
    virtual bool IsNegativeOne() const = 0;
    virtual bool IsZero() const = 0;
    virtual bool IsNegative() const = 0;
    virtual bool IsExpression() const;
    virtual bool IsValue() const;
    virtual bool IsNamed() const;
    virtual Pointer Copy() const = 0;
    virtual Pointer Invert() const = 0;
    virtual bool ScalarsAdd(Pointer other) const = 0;
    virtual bool PowersAdd(Pointer other) const = 0;
    virtual bool Equals(Pointer other) const = 0;

    virtual bool SortProduct(Pointer) const
    {
        return false;
    }

    virtual bool IsTrig() const
    {
        return false;
    }

    size_t GetDisplayWidth() const;
};


bool operator<(typename Symbol::Pointer left, typename Symbol::Pointer right);


std::ostream & operator<<(
    std::ostream &output,
    typename Symbol::Pointer symbol);


std::ostream & operator<<(
    std::ostream &output,
    const Symbol &symbol);


typename Symbol::Pointer operator+(
    typename Symbol::Pointer left,
    typename Symbol::Pointer right);


typename Symbol::Pointer operator-(
    typename Symbol::Pointer left,
    typename Symbol::Pointer right);


typename Symbol::Pointer operator*(
    typename Symbol::Pointer left,
    typename Symbol::Pointer right);


typename Symbol::Pointer operator/(
    typename Symbol::Pointer left,
    typename Symbol::Pointer right);


std::ostream & operator<<(std::ostream &output, const Op &op);


bool IsTrigName(const std::string &name);


class Arg: public std::string
{
private:
    Arg(const std::string &name);

    static std::shared_ptr<Arg> CreateShared_(const std::string &name);

public:
    void ClearValue();

    void SetValue(double value);

    std::optional<double> GetValue() const;

    static std::shared_ptr<Arg> Get(const std::string &name);


private:
    std::optional<double> value_;
};


double GetTrigValue(const std::string &name, double arg);


class SymbolName
{
public:
    SymbolName(const std::string &name);

    SymbolName(const std::string &name, const std::string &arg);

    std::ostream & ToStream(std::ostream &output, const Symbol &power) const;

    std::string GetName() const;

    bool operator==(const SymbolName &other) const;

    bool operator!=(const SymbolName &other) const;

    bool operator<(const SymbolName &other) const;

    bool IsTrig() const
    {
        return this->isTrig_;
    }

    template<typename T>
    std::optional<T> GetValue() const
    {
        auto argValue = this->arg_->GetValue();

        if (!argValue)
        {
            return {};
        }

        double value;

        if (!this->isTrig_)
        {
            value = *argValue;
        }
        else
        {
            value = GetTrigValue(this->name_, *argValue);
        }

        if constexpr (std::is_integral_v<T>)
        {
            return static_cast<T>(std::round(value));
        }
        else
        {
            return static_cast<T>(value);
        }
    }

    std::shared_ptr<Arg> GetArg()
    {
        return this->arg_;
    }

private:
    std::string name_;
    std::shared_ptr<Arg> arg_;
    bool isTrig_;
};


std::ostream & operator<<(std::ostream &output, const SymbolName &symbolName);


class S: public std::shared_ptr<Symbol>
{
public:
    using Base = std::shared_ptr<Symbol>;

    S();

    S(const std::shared_ptr<Symbol> &pointer);

    S(int value);

    S(const std::string &name);

    S(const std::string &name, const std::string &arg);
};


S operator+(S left, S right);
S operator-(S left, S right);
S operator*(S left, S right);
S operator/(S left, S right);
S operator^(S left, S right);
