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
    this->cosSum.name = S("cos", angleSumName);
    this->sinSum.name = S("sin", angleSumName);

    auto angleDifferenceName = fmt::format("{} - {}", first, second);
    this->cosDifference.name = S("cos", angleDifferenceName);
    this->sinDifference.name = S("sin", angleDifferenceName);

    sum = positive * a;
    difference = negative * a;

    this->cosSum.expression = sum(0, 0);
    this->sinSum.expression = sum(1, 0);

    this->cosDifference.expression = difference(0, 0);
    this->sinDifference.expression = difference(1, 0);
}


std::ostream & operator<<(
    std::ostream &output,
    const SumAndDifference &value)
{
    output << value.cosSum.name << ": " << value.cosSum.expression << '\n';
    output << value.sinSum.name << ": " << value.sinSum.expression << '\n';

    output << value.cosDifference.name << ": "
        << value.cosDifference.expression << '\n';

    output << value.sinDifference.name << ": "
        << value.sinDifference.expression << '\n';

    return output;
}


void CheckIdentity(Symbol::Pointer &element, const Identity &identity)
{
    if (element->Equals(identity.expression))
    {
        element = identity.name;
    }
    else if ((element * -1)->Equals(identity.expression))
    {
        element = identity.name * -1;
    }
}


void ReplaceElement(Matrix &matrix, size_t row, size_t column)
{
    auto &element = matrix(row, column);

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

    CheckIdentity(element, sums.sinSum);
    CheckIdentity(element, sums.cosSum);
    CheckIdentity(element, sums.sinDifference);
    CheckIdentity(element, sums.cosDifference);
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
