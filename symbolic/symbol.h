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

    virtual Op GetOp() const;

    virtual Pointer operator+(Pointer other) const = 0;
    virtual Pointer operator-(Pointer other) const = 0;
    virtual Pointer operator*(Pointer other) const = 0;
    virtual Pointer operator/(Pointer other) const = 0;

    virtual Pointer GetScalar() const = 0;
    virtual Pointer ClearScalar() const = 0;
    virtual Pointer MultiplyScalar(Pointer scalar) const = 0;
    virtual bool IsExpression() const;
    virtual bool IsValue() const;
    virtual bool IsNamed() const;

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

private:
    std::string name_;
    std::string arg_;
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

    S(const char *name);

    S(const char *name, const char *arg);
};


S operator+(S left, S right);
S operator-(S left, S right);
S operator*(S left, S right);
S operator/(S left, S right);
