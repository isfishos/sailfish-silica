// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_AUTOFILL_H
#define SAILFISH_SILICA_PLUGIN_AUTOFILL_H

#include <QObject>
#include <QStringList>
#include <QBasicTimer>
#include <qqml.h>

class QQuickItem;

class AutoFill : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QString partialText READ partialText WRITE setPartialText NOTIFY partialTextChanged)
    Q_PROPERTY(int maximumSuggestions READ maximumSuggestions WRITE setMaximumSuggestions NOTIFY maximumSuggestionsChanged)
    Q_PROPERTY(bool canRemove READ canRemove WRITE setCanRemove NOTIFY canRemoveChanged)
    Q_PROPERTY(QStringList suggestions READ suggestions WRITE setSuggestions NOTIFY suggestionsChanged)
    Q_PROPERTY(QQuickItem* inputItem READ inputItem WRITE setInputItem NOTIFY inputItemChanged)
public:
    explicit AutoFill(QObject *parent = nullptr);

    QString key() const;
    void setKey(const QString &k);

    QString partialText() const;
    void setPartialText(const QString &t);

    int maximumSuggestions() const;
    void setMaximumSuggestions(int v);

    bool canRemove() const;
    void setCanRemove(bool v);

    QStringList suggestions() const;
    void setSuggestions(const QStringList &s);

    QQuickItem *inputItem() const;
    void setInputItem(QQuickItem *i);

    Q_INVOKABLE void insert(const QString &suggestion);
    Q_INVOKABLE void remove(const QString &suggestion);
    Q_INVOKABLE void save();

    static AutoFill *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void keyChanged();
    void partialTextChanged();
    void maximumSuggestionsChanged();
    void canRemoveChanged();
    void suggestionsChanged();
    void inputItemChanged();
    void suggestionRemoved(const QString &suggestion);

private:
    void scheduleRecompute();
    void recomputeSuggestions();
    void load();
    void persist();

    void timerEvent(QTimerEvent *ev) override;

    QString m_key;
    QString m_partial;
    int m_max = 10;
    bool m_canRemove = true;
    QStringList m_suggestions;
    QQuickItem *m_input = nullptr;
    bool m_partialExplicit = false;
    QBasicTimer m_debounce;
};

QML_DECLARE_TYPEINFO(AutoFill, QML_HAS_ATTACHED_PROPERTIES)

#endif // SAILFISH_SILICA_PLUGIN_AUTOFILL_H
