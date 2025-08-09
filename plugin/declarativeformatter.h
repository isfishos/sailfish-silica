// SPDX-License-Identifier: LGPL-2.1-only

#ifndef SAILFISH_SILICA_PLUGIN_DECLARATIVEFORMATTER_H
#define SAILFISH_SILICA_PLUGIN_DECLARATIVEFORMATTER_H

#include <QObject>
#include <QDateTime>
#include <QString>

class DeclarativeFormatter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString listSeparator READ listSeparator CONSTANT)

public:
    explicit DeclarativeFormatter(QObject *parent = nullptr);

    enum FormatType {
        Timepoint,
        DateFull,
        DateMediumWithoutYear,
        TimeElapsed,
        TimepointRelative
    };
    Q_ENUM(FormatType)

    enum DurationType {
        DurationAuto,
        DurationShort,
        DurationLong
    };
    Q_ENUM(DurationType)

    enum ArticleType {
        ArticleDefinite,
        ArticleIndefinite
    };
    Q_ENUM(ArticleType)

    enum TextFormatType {
        Ascii7Bit,
        PortableFilename
    };
    Q_ENUM(TextFormatType)

    QString listSeparator() const;

    Q_INVOKABLE QString formatDate(const QDateTime &dateTime, FormatType formatType);
    Q_INVOKABLE QString formatArticle(ArticleType articleType);
    Q_INVOKABLE QString formatDuration(int seconds, DurationType formatType = DurationAuto);
    Q_INVOKABLE QString formatFileSize(qint64 bytes, int precision = 1);
    Q_INVOKABLE QString formatText(const QString &input, TextFormatType formatType);
    Q_INVOKABLE bool _needsSpaceBetweenNames(const QString &first, const QString &second);
    Q_INVOKABLE QString _joinNames(const QString &first, const QString &second);
    Q_INVOKABLE QString trId(const QString &id, const QString &catalog, int n = -1, const QString &localeName = "");
    Q_INVOKABLE Qt::LayoutDirection textDirection(const QString &text);

private:
    QString formatRelativeTime(const QDateTime &dateTime);
    QString formatDurationInternal(int seconds, DurationType formatType);
    QString formatFileSizeInternal(qint64 bytes, int precision);
    QString formatTextInternal(const QString &input, TextFormatType formatType);
    Qt::LayoutDirection determineTextDirection(const QString &text);

    QString m_listSeparator;
};

#endif // SAILFISH_SILICA_PLUGIN_DECLARATIVEFORMATTER_H
