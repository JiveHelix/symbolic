/**
  * @file matrix.h
  *
  * @brief Symbolic matrix manipulation.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include "symbol.h"
#include "rank.h"


class Matrix
{
public:
    using Pointer = typename Symbol::Pointer;

    Matrix(size_t rows, size_t columns);

    S & operator()(size_t row, size_t column);

    const S & operator()(size_t row, size_t column) const;

    template<Op op>
    Matrix & ScalarOperatorAssign_(S scalar)
    {
        static_assert(IsValidOperator<op>::value);

        for (size_t column = 0; column < this->columns_; ++column)
        {
            auto &columnValues = this->values_[column];

            for (size_t row = 0; row < this->rows_; ++row)
            {
                if constexpr (op == Op::multiply)
                {
                    columnValues[row] = columnValues[row] * scalar;
                }
                else if constexpr (op == Op::divide)
                {
                    columnValues[row] = columnValues[row] / scalar;
                }
                else if constexpr (op == Op::add)
                {
                    columnValues[row] = columnValues[row] + scalar;
                }
                else if constexpr (op == Op::subtract)
                {
                    columnValues[row] = columnValues[row] - scalar;
                }
            }
        }

        return *this;
    }

    Matrix & operator+=(S scalar);

    Matrix & operator-=(S scalar);

    Matrix & operator*=(S scalar);

    Matrix & operator/=(S scalar);

    Matrix operator+(S scalar) const;

    Matrix operator-(S scalar) const;

    Matrix operator*(S scalar) const;

    Matrix operator/(S scalar) const;

    Matrix operator*(const Matrix &other) const;

    size_t GetRowCount() const;

    size_t GetColumnCount() const;

    size_t GetColumnWidth() const;

    std::ostream & ToStream(std::ostream &output) const;

    Matrix & operator=(const std::vector<S> &symbols);

    template<typename ...Args>
    void Assign(Args&&... args)
    {
        if (sizeof...(Args) != this->rows_ * this->columns_)
        {
            throw std::out_of_range("Unexpected symbol count");
        }

        std::vector<S> symbols;

        (symbols.push_back(args), ...);

        this->operator=(symbols);
    }

private:
    size_t rows_;
    size_t columns_;

    using Column = Rank<S>;

    Rank<Column> values_;
};


std::ostream & operator<<(std::ostream &output, const Matrix &matrix);
