#include "pswvalidator.h"

PswValidator::PswValidator(QObject *parent):
    QValidator(parent)
{
}

QValidator::State PswValidator::validate(QString &input, int &pos) const
{
    if (!input.compare("ok")) return QValidator::Acceptable;
        else return QValidator::Intermediate;
}
