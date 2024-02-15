#include <cassert>
#include "symbolic/angle_sums.h"
#include "symbolic/expression.h"
#include "symbolic/named.h"


SumAndDifference::SumAndDifference(
    const std::string &first,
    const std::string &second)
{
    Matrix sum(2, 1);
    Matrix difference(2, 1);
    Matrix a(2, 1);
    Matrix positive(2, 2);
    Matrix negative(2, 2);

    auto sinFirst = S{"sin", first};
    auto cosFirst = S{"cos", first};

    auto sinSecond = S{"sin", second};
    auto cosSecond = S{"cos", second};

    a.Assign(
        cosFirst,
        sinFirst);

    positive.Assign(
        cosSecond, -1 * sinSecond,
        sinSecond, cosSecond);

    negative.Assign(
        cosSecond, sinSecond,
        -1 * sinSecond, cosSecond);

    auto angleSumName = fmt::format("{} + {}", first, second);
    this->cosSumName = S("cos", angleSumName);
    this->sinSumName = S("sin", angleSumName);

    auto angleDifferenceName = fmt::format("{} - {}", first, second);
    this->cosDifferenceName = S("cos", angleDifferenceName);
    this->sinDifferenceName = S("sin", angleDifferenceName);

    sum = positive * a;
    difference = negative * a;

    this->cosSum = sum(0, 0);
    this->sinSum = sum(1, 0);

    this->cosDifference = difference(0, 0);
    this->sinDifference = difference(1, 0);
}


std::ostream & operator<<(
    std::ostream &output,
    const SumAndDifference &value)
{
    output << value.cosSumName << ": " << value.cosSum << '\n';
    output << value.sinSumName << ": " << value.sinSum << '\n';
    output << value.cosDifferenceName << ": " << value.cosDifference << '\n';
    output << value.sinDifferenceName << ": " << value.sinDifference << '\n';

    return output;
}


void ReplaceElement(Matrix &matrix, size_t row, size_t column)
{
    auto element = matrix(row, column);

    if (!element->IsExpression())
    {
        return;
    }

    auto expression =
        dynamic_cast<Expression *>(element.get());

    assert(expression);

    if (!expression->IsTrig())
    {
        return;
    }

    if (!expression->GetLeft()->IsExpression())
    {
        return;
    }

    auto left =
        dynamic_cast<Expression *>(expression->GetLeft().get());
    
    auto leftNamed =
        dynamic_cast<Named *>(left->GetLeft().get());

    auto rightNamed =
        dynamic_cast<Named *>(left->GetRight().get());

    if (!leftNamed || !rightNamed)
    {
        return;
    }

    SumAndDifference sums(
        *leftNamed->GetArg(),
        *rightNamed->GetArg());

    assert(sums.sinSum->Equals(sums.sinSum));

    if (element->Equals(sums.sinSum))
    {
        matrix(row, column) = sums.sinSumName; 
    }
    else if ((element * -1)->Equals(sums.sinSum))
    {
        matrix(row, column) = sums.sinSumName * -1; 
    }

    else if (element->Equals(sums.cosSum))
    {
        matrix(row, column) = sums.cosSumName; 
    }
    else if ((element * -1)->Equals(sums.cosSum))
    {
        matrix(row, column) = sums.cosSumName * -1; 
    }

    else if (element->Equals(sums.cosDifference))
    {
        matrix(row, column) = sums.cosDifferenceName; 
    }
    else if ((element * -1)->Equals(sums.cosDifference))
    {
        matrix(row, column) = sums.cosDifferenceName * -1; 
    }

    else if (element->Equals(sums.sinDifference))
    {
        matrix(row, column) = sums.sinDifferenceName; 
    }
    else if ((element * -1)->Equals(sums.sinDifference))
    {
        matrix(row, column) = sums.sinDifferenceName * -1; 
    }
}


Matrix ReplaceAngleSums(const Matrix &matrix)
{
    Matrix result = matrix;

    for (size_t column = 0; column < matrix.GetColumnCount(); ++column)
    {
        for (size_t row = 0; row < matrix.GetRowCount(); ++row)
        {
            ReplaceElement(result, row, column);
        }
    }

    return result;
}
