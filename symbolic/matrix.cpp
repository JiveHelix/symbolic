/**
  * @file matrix.cpp
  *
  * @brief Implements the matrix operations.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#include "symbolic/matrix.h"
#include "symbolic/settings.h"

#include <fmt/core.h>
#include <iostream>


using Pointer = typename Symbol::Pointer;


Matrix::Matrix(size_t rows, size_t columns)
    :
    rows_(rows),
    columns_(columns),
    values_(columns, rows)
{

}

S & Matrix::operator()(size_t row, size_t column)
{
    return this->values_[column][row];
}

const S & Matrix::operator()(size_t row, size_t column) const
{
    return this->values_[column][row];
}

Matrix & Matrix::operator+=(S scalar)
{
    return this->template ScalarOperatorAssign_<Op::add>(scalar);
}

Matrix & Matrix::operator-=(S scalar)
{
    return this->template ScalarOperatorAssign_<Op::subtract>(scalar);
}

Matrix & Matrix::operator*=(S scalar)
{
    return this->template ScalarOperatorAssign_<Op::multiply>(scalar);
}

Matrix & Matrix::operator/=(S scalar)
{
    return this->template ScalarOperatorAssign_<Op::divide>(scalar);
}

Matrix Matrix::operator+(S scalar) const
{
    Matrix result = *this;
    result += scalar;
    return result;
}

Matrix Matrix::operator-(S scalar) const
{
    Matrix result = *this;
    result -= scalar;
    return result;
}

Matrix Matrix::operator*(S scalar) const
{
    Matrix result = *this;
    result *= scalar;
    return result;
}

Matrix Matrix::operator/(S scalar) const
{
    Matrix result = *this;
    result /= scalar;
    return result;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    if (this->columns_ != other.rows_)
    {
        throw std::runtime_error("Incompatible dimensions");
    }

    Matrix result(this->rows_, other.columns_);

    for (size_t column = 0; column < result.GetColumnCount(); ++column)
    {
        for (size_t row = 0; row < result.GetRowCount(); ++row)
        {
            S element = this->operator()(row, 0) * other(0, column);

            for (size_t i = 1; i < this->columns_; ++i)
            {
                auto left = this->operator()(row, i);
                auto right = other.operator()(i, column);
                auto product = left * right;
                auto sum = element + product;
                element = sum;
            }

            result(row, column) = element;
        }
    }

    return result;
}

size_t Matrix::GetRowCount() const
{
    return this->rows_;
}

size_t Matrix::GetColumnCount() const
{
    return this->columns_;
}

size_t Matrix::GetColumnWidth() const
{
    size_t width = 0;

    for (size_t column = 0; column < this->columns_; ++column)
    {
        width = std::max(width, this->values_[column].GetColumnWidth());
    }

    return width;
}

std::ostream & Matrix::ToStreamCompact(std::ostream &output) const
{
    std::vector<size_t> widths;

    for (size_t column = 0; column < this->columns_; ++column)
    {
        widths.push_back(this->values_[column].GetColumnWidth());
    }

    for (size_t row = 0; row < this->rows_; ++row)
    {
        output << "[";

        for (size_t column = 0; column < this->columns_; ++column)
        {
            std::ostringstream stringStream;
            this->operator()(row, column)->ToStream(stringStream);

            output << fmt::format(
                "{:^{}}",
                stringStream.str(),
                widths[column] + 2);
        }

        output << "]\n";
    }

    return output;
}

std::ostream & Matrix::ToStream(std::ostream &output) const
{
    if (settings::printCompact)
    {
        return this->ToStreamCompact(output);
    }

    size_t columnWidth = this->GetColumnWidth() + 2;

    for (size_t row = 0; row < this->rows_; ++row)
    {
        output << "[";

        for (size_t column = 0; column < this->columns_; ++column)
        {
            std::ostringstream stringStream;
            this->operator()(row, column)->ToStream(stringStream);

            output << fmt::format(
                "{:^{}}",
                stringStream.str(),
                columnWidth);
        }

        output << "]\n";
    }

    return output;
}

Matrix & Matrix::operator=(const std::vector<S> &symbols)
{
    if (symbols.size() != this->rows_ * this->columns_)
    {
        throw std::out_of_range("Unexpected symbol count");
    }

    for (size_t i = 0; i < this->rows_; ++i)
    {
        for (size_t j = 0; j < this->columns_; ++j)
        {
            this->operator()(i, j) =
                symbols[i * this->columns_ + j];
        }
    }

    return *this;
}


std::ostream & operator<<(std::ostream &output, const Matrix &matrix)
{
    return matrix.ToStream(output);
}
