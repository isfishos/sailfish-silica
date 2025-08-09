// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_PROXYVALIDATOR_H
#define SAILFISH_SILICA_PLUGIN_PROXYVALIDATOR_H

#include <QValidator>
#include <QPointer>

class ProxyValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(bool strictValidation READ strictValidation WRITE setStrictValidation NOTIFY strictValidationChanged)
    Q_PROPERTY(QValidator *validator READ validator WRITE setValidator NOTIFY validatorChanged)

public:
    explicit ProxyValidator(QObject *parent = nullptr);

    bool strictValidation() const { return m_strictValidation; }
    void setStrictValidation(bool strict);
    QValidator *validator() const { return m_validator; }
    void setValidator(QValidator *validator);

    QValidator::State validate(QString &input, int &pos) const override;

Q_SIGNALS:
    void strictValidationChanged();
    void validatorChanged();

private:
    bool m_strictValidation = false;
    QPointer<QValidator> m_validator;
};

#endif // SAILFISH_SILICA_PLUGIN_PROXYVALIDATOR_H
