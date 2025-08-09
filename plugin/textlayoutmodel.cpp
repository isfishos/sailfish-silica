// SPDX-License-Identifier: LGPL-2.1-only

#include "textlayoutmodel.h"
#include <QTextLayout>
#include <QTextOption>

TextLayoutModel::TextLayoutModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void TextLayoutModel::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        updateLayout();
        emit textChanged();
    }
}

void TextLayoutModel::setFont(const QFont &font)
{
    if (m_font != font) {
        m_font = font;
        updateLayout();
        emit fontChanged();
    }
}

void TextLayoutModel::setWidth(qreal width)
{
    if (!qFuzzyCompare(m_width, width)) {
        m_width = width;
        updateLayout();
        emit widthChanged();
    }
}

void TextLayoutModel::setWrapMode(int mode)
{
    if (m_wrapMode != mode) {
        m_wrapMode = mode;
        updateLayout();
        emit wrapModeChanged();
    }
}

void TextLayoutModel::setMaximumLineCount(int count)
{
    if (m_maximumLineCount != count) {
        m_maximumLineCount = count;
        updateLayout();
        emit maximumLineCountChanged();
    }
}

int TextLayoutModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_lines.size();
}

QVariant TextLayoutModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_lines.size()) {
        return QVariant();
    }

    const TextLineInfo &line = m_lines.at(index.row());

    switch (role) {
    case LineNumberRole:
        return line.lineNumber;
    case TextRole:
        return line.text;
    case LengthRole:
        return line.length;
    case WidthRole:
        return line.width;
    case HeightRole:
        return line.height;
    case ElidedRole:
        return line.elided;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TextLayoutModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[LineNumberRole] = "lineNumber";
    roles[TextRole] = "text";
    roles[LengthRole] = "length";
    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    roles[ElidedRole] = "elided";
    return roles;
}

void TextLayoutModel::updateLayout()
{
    beginResetModel();
    m_lines.clear();

    if (m_text.isEmpty() || m_width <= 0) {
        endResetModel();
        emit lineCountChanged();
        return;
    }

    QTextLayout layout(m_text, m_font);
    QTextOption option;

    // Set wrap mode
    switch (m_wrapMode) {
    case 1: // WordWrap
        option.setWrapMode(QTextOption::WordWrap);
        break;
    case 2: // WrapAnywhere
        option.setWrapMode(QTextOption::WrapAnywhere);
        break;
    default:
        option.setWrapMode(QTextOption::NoWrap);
        break;
    }

    layout.setTextOption(option);
    layout.beginLayout();

    int lineNumber = 0;
    QTextLine line = layout.createLine();

    while (line.isValid() && (m_maximumLineCount == -1 || lineNumber < m_maximumLineCount)) {
        line.setLineWidth(m_width);

        TextLineInfo lineInfo;
        lineInfo.lineNumber = lineNumber;
        lineInfo.text = m_text.mid(line.textStart(), line.textLength());
        lineInfo.length = line.textLength();
        lineInfo.width = line.naturalTextWidth();
        lineInfo.height = line.height();
        lineInfo.elided = false;

        m_lines.append(lineInfo);

        lineNumber++;
        line = layout.createLine();
    }

    layout.endLayout();

    // Check if we need to elide the last line
    if (m_maximumLineCount > 0 && m_lines.size() >= m_maximumLineCount && line.isValid()) {
        if (!m_lines.isEmpty()) {
            m_lines.last().elided = true;
        }
    }

    endResetModel();
    emit lineCountChanged();
}
