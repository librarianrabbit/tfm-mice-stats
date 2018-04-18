#include "micenamevalidator.h"

MiceNameValidator::MiceNameValidator(QObject* parent) :
    QValidator(parent)
{
}

QValidator::State MiceNameValidator::validate(QString& input, int& pos) const
{
    if (input.size() > 0)
    {
        if (input[0].isLower()) input[0] = input[0].toUpper();
        input = input.remove(QRegExp("[^a-zA-Z]"));
    }

    return Acceptable;
}
