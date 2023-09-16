/**
  * @file rank.h
  *
  * @brief The Rank class holds a column, or a row of columns.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 16 Sep 2023
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once


#include <type_traits>
#include <vector>


template<typename T, typename Enable = void>
struct IsRank: std::false_type {};

template<typename T>
struct IsRank
<
    T,
    std::enable_if_t<T::isRank>
>: std::true_type {};


template<typename Value>
class Rank
{
public:
    static constexpr bool isRank = true;

    Rank(size_t size)
        :
        values_(size)
    {

    }

    Rank(size_t size, size_t valueSize)
        :
        values_()
    {
        if constexpr (IsRank<Value>::value)
        {
            while (size--)
            {
                this->values_.emplace_back(valueSize);
            }
        }
    }

    Value & operator[](size_t index)
    {
        return this->values_.at(index);
    }

    const Value & operator[](size_t index) const
    {
        return this->values_.at(index);
    }

    size_t GetSize() const
    {
        return this->values_.size();
    }

    size_t GetColumnWidth() const
    {
        if constexpr (IsRank<Value>::value)
        {
            return 0;
        }

        size_t width = 0;

        for (auto &value: this->values_)
        {
            width = std::max(width, value->GetDisplayWidth());
        }

        return width;
    }

private:
    std::vector<Value> values_;
};


