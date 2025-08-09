// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativeclipboard.h"
#include <QGuiApplication>
#include <QClipboard>

DeclarativeClipboard::DeclarativeClipboard(QObject *parent)
    : QObject(parent)
{
    if (auto cb = QGuiApplication::clipboard()) {
        connect(cb, &QClipboard::dataChanged, this, [this, cb]{
            const QString t = cb->text();
            const bool h = !t.isEmpty();
            if (m_text != t) {
                m_text = t;
                emit textChanged();
            }
            if (m_hasText != h) {
                m_hasText = h;
                emit hasTextChanged();
            }
        });
    }
}

QString DeclarativeClipboard::text() const {
     return m_text;
}

void DeclarativeClipboard::setText(const QString &t)
{
    if (m_text == t) return;
    m_text = t; emit textChanged();
    if (auto cb = QGuiApplication::clipboard()) {
        cb->setText(t);
    }
    const bool h = !t.isEmpty();
    if (m_hasText != h) {
        m_hasText = h;
        emit hasTextChanged();
    }
}

bool DeclarativeClipboard::hasText() const {
    return m_hasText;
}
