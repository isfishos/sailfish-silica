// SPDX-License-Identifier: LGPL-2.1-only

#include "proxyvalidator.h"

ProxyValidator::ProxyValidator(QObject *parent)
    : QValidator(parent)
{
}

void ProxyValidator::setStrictValidation(bool strict)
{
    if (m_strictValidation != strict) {
        m_strictValidation = strict;
        emit strictValidationChanged();
    }
}

void ProxyValidator::setValidator(QValidator *validator)
{
    if (m_validator != validator) {
        m_validator = validator;
        emit validatorChanged();
    }
}

QValidator::State ProxyValidator::validate(QString &input, int &pos) const
{
    if (!m_validator) {
        return m_strictValidation ? Invalid : Acceptable;
    }

    QValidator::State state = m_validator->validate(input, pos);

    if (m_strictValidation) {
        // Map Intermediate to Invalid when strict validation is enabled
        if (state == Intermediate) {
            return Invalid;
        }
    }

    return state;
}
