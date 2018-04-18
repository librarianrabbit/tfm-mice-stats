#ifndef MICENAMEVALIDATOR_H
#define MICENAMEVALIDATOR_H

#include <QValidator>

class MiceNameValidator : public QValidator
{
    Q_OBJECT
public:
    explicit MiceNameValidator(QObject* parent = 0);

    State validate(QString& input, int& pos) const;
};

#endif // MICENAMEVALIDATOR_H
