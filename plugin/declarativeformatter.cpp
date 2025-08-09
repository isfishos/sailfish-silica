// SPDX-License-Identifier: LGPL-2.1-only

#include "declarativeformatter.h"
#include <QLocale>
#include <QCoreApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDir>

DeclarativeFormatter::DeclarativeFormatter(QObject *parent)
    : QObject(parent)
{
    // Initialize list separator from locale
    QLocale locale;
    m_listSeparator = ", "; // Simple list separator
}

QString DeclarativeFormatter::listSeparator() const
{
    return m_listSeparator;
}

QString DeclarativeFormatter::formatDate(const QDateTime &dateTime, FormatType formatType)
{
    QLocale locale;

    switch (formatType) {
        case Timepoint:
            return locale.toString(dateTime.time(), QLocale::ShortFormat);
        case DateFull:
            return locale.toString(dateTime.date(), QLocale::LongFormat);
        case DateMediumWithoutYear:
            return locale.toString(dateTime.date(), "MMM d");
        case TimeElapsed:
        case TimepointRelative:
            return formatRelativeTime(dateTime);
        default:
            return locale.toString(dateTime, QLocale::ShortFormat);
    }
}

QString DeclarativeFormatter::formatArticle(ArticleType articleType)
{
    // This would typically use translation catalogs
    switch (articleType) {
        case ArticleDefinite:
            return QCoreApplication::translate("Formatter", "the");
        case ArticleIndefinite:
            return QCoreApplication::translate("Formatter", "a");
        default:
            return QString();
    }
}

QString DeclarativeFormatter::formatDuration(int seconds, DurationType formatType)
{
    return formatDurationInternal(seconds, formatType);
}

QString DeclarativeFormatter::formatFileSize(qint64 bytes, int precision)
{
    return formatFileSizeInternal(bytes, precision);
}

QString DeclarativeFormatter::formatText(const QString &input, TextFormatType formatType)
{
    return formatTextInternal(input, formatType);
}

bool DeclarativeFormatter::_needsSpaceBetweenNames(const QString &first, const QString &second)
{
    // Simple heuristic: if both names end with letters, they need a space
    if (first.isEmpty() || second.isEmpty()) {
        return false;
    }

    QChar firstLast = first.at(first.length() - 1);
    QChar secondFirst = second.at(0);

    return firstLast.isLetter() && secondFirst.isLetter();
}

QString DeclarativeFormatter::_joinNames(const QString &first, const QString &second)
{
    if (first.isEmpty()) {
        return second;
    }
    if (second.isEmpty()) {
        return first;
    }

    if (_needsSpaceBetweenNames(first, second)) {
        return first + " " + second;
    } else {
        return first + second;
    }
}

QString DeclarativeFormatter::trId(const QString &id, const QString &catalog, int n, const QString &localeName)
{
    // TODO: Implement proper translator loading
    Q_UNUSED(catalog)
    Q_UNUSED(localeName)

    if (n >= 0) {
        return QCoreApplication::translate("Formatter", id.toUtf8().constData(), nullptr, n);
    } else {
        return QCoreApplication::translate("Formatter", id.toUtf8().constData());
    }
}

Qt::LayoutDirection DeclarativeFormatter::textDirection(const QString &text)
{
    return determineTextDirection(text);
}

QString DeclarativeFormatter::formatRelativeTime(const QDateTime &dateTime)
{
    QDateTime now = QDateTime::currentDateTime();
    qint64 secondsDiff = dateTime.secsTo(now);

    if (secondsDiff < 60) {
        return QCoreApplication::translate("Formatter", "just now");
    } else if (secondsDiff < 3600) {
        int minutes = secondsDiff / 60;
        return QCoreApplication::translate("Formatter", "%n minute(s) ago", nullptr, minutes);
    } else if (secondsDiff < 86400) {
        int hours = secondsDiff / 3600;
        return QCoreApplication::translate("Formatter", "%n hour(s) ago", nullptr, hours);
    } else {
        int days = secondsDiff / 86400;
        return QCoreApplication::translate("Formatter", "%n day(s) ago", nullptr, days);
    }
}

QString DeclarativeFormatter::formatDurationInternal(int seconds, DurationType formatType)
{
    if (formatType == DurationAuto) {
        if (seconds < 60) {
            formatType = DurationShort;
        } else if (seconds < 3600) {
            formatType = DurationShort;
        } else {
            formatType = DurationLong;
        }
    }

    if (formatType == DurationShort) {
        if (seconds < 60) {
            return QString::number(seconds) + "s";
        } else if (seconds < 3600) {
            return QString::number(seconds / 60) + "m";
        } else {
            return QString::number(seconds / 3600) + "h";
        }
    } else { // DurationLong
        if (seconds < 60) {
            return QCoreApplication::translate("Formatter", "%n second(s)", nullptr, seconds);
        } else if (seconds < 3600) {
            return QCoreApplication::translate("Formatter", "%n minute(s)", nullptr, seconds / 60);
        } else {
            return QCoreApplication::translate("Formatter", "%n hour(s)", nullptr, seconds / 3600);
        }
    }
}

QString DeclarativeFormatter::formatFileSizeInternal(qint64 bytes, int precision)
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;

    QLocale locale;

    if (bytes < KB) {
        return QCoreApplication::translate("Formatter", "%n byte(s)", nullptr, bytes);
    } else if (bytes < MB) {
        double kb = static_cast<double>(bytes) / KB;
        return locale.toString(kb, 'f', precision) + " kB";
    } else if (bytes < GB) {
        double mb = static_cast<double>(bytes) / MB;
        return locale.toString(mb, 'f', precision) + " MB";
    } else {
        double gb = static_cast<double>(bytes) / GB;
        return locale.toString(gb, 'f', precision) + " GB";
    }
}

QString DeclarativeFormatter::formatTextInternal(const QString &input, TextFormatType formatType)
{
    switch (formatType) {
        case Ascii7Bit: {
            // Convert to ASCII, removing diacritics where possible
            QString result;
            for (QChar ch : input) {
                if (ch.unicode() < 128) {
                    result.append(ch);
                } else {
                    // Simple ASCII approximation
                    switch (ch.unicode()) {
                        case 0x00E4: result.append('a'); break; // ä
                        case 0x00F6: result.append('o'); break; // ö
                        case 0x00FC: result.append('u'); break; // ü
                        case 0x00C4: result.append('A'); break; // Ä
                        case 0x00D6: result.append('O'); break; // Ö
                        case 0x00DC: result.append('U'); break; // Ü
                        default: result.append('?'); break;
                    }
                }
            }
            return result;
        }
        case PortableFilename: {
            // Remove invalid filename characters
            QString result = input;
            QRegularExpression invalidChars("[<>:\"/\\|?*]");
            result.remove(invalidChars);
            return result;
        }
        default:
            return input;
    }
}

Qt::LayoutDirection DeclarativeFormatter::determineTextDirection(const QString &text)
{
    // Simple RTL detection for common scripts
    for (QChar ch : text) {
        ushort unicode = ch.unicode();
        if (unicode >= 0x0590 && unicode <= 0x05FF) { // Hebrew
            return Qt::RightToLeft;
        }
        if (unicode >= 0x0600 && unicode <= 0x06FF) { // Arabic
            return Qt::RightToLeft;
        }
        if (unicode >= 0x0750 && unicode <= 0x077F) { // Arabic Supplement
            return Qt::RightToLeft;
        }
        if (unicode >= 0x08A0 && unicode <= 0x08FF) { // Arabic Extended-A
            return Qt::RightToLeft;
        }
    }
    return Qt::LeftToRight;
}
