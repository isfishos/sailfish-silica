/****************************************************************************
 **
 ** Copyright (C) 2013, 2015 - 2017 Jolla Ltd.
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 **
 ** This file is part of the Qt Components project.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef SILICA_THEME_ICON_RESOLVER_H
#define SILICA_THEME_ICON_RESOLVER_H

#include <silicaglobal.h>
#include <QImage>
#include <QString>
#include "silicatheme.h"

namespace Silica {

class IconInfoPrivate;

class SAILFISH_SILICA_EXPORT IconInfo {
public:
    enum IconType {
        ColorIcon,
        MonochromeIcon
    };

    IconInfo();
    IconInfo(const QString &filePath, IconType iconType);
    IconInfo(const IconInfo &other);
    IconInfo &operator =(const IconInfo& other);

    ~IconInfo();

    QString filePath() const;
    IconType iconType() const;

private:
    IconInfoPrivate *d_ptr;
};

class ThemeIconResolverPrivate;

/**
 * \brief Allows to request theme icons.
 */
class SAILFISH_SILICA_EXPORT ThemeIconResolver
{
public:
    enum InitFlags {
        NoContent = 0x0,
        LoadDefaultTheme = 0x1
    };

    ThemeIconResolver();
    ThemeIconResolver(InitFlags initFlags);
    virtual ~ThemeIconResolver();

    /**
     * @brief resolvePath Resolves absolute path of the an icon id.
     * @param id          Identifier of the image.
     * @return            Returns absolute path of the \a id. If icon is not found an empty string is returned.
     */
    QString resolvePath(const QString &id) const;
    QString resolvePath(const QString &id, Silica::Theme::ColorScheme colorScheme) const;

    /**
     * @brief resolveIcon Resolves icon info from an icon id.
     * @param id          Identifier of the image.
     * @return            Returns info object for the lookup result. If icon is not found, it has an empty path.
     */
    IconInfo resolveIcon(const QString &id) const;
    IconInfo resolveIcon(const QString &id, Silica::Theme::ColorScheme colorScheme) const;

    /**
     * @brief addIconRoot Add an icon root to be used for icon lookups.
     *
     * This can include multiple 'zN.M' subdirectories where N.M signify the pixel ratio
     * using two decimals, or one if last is zero. The root and the pixel ratio subdirectories
     * can contain 'icons' and 'icons-monochrome' directories for the icon content.
     * @param path        Path of the icon root.
     */
    void addIconRoot(const QString &path);

    // deprecated
    static void setTestModeDirectories(const QString &testPath, const QString &testSubDir);

private:
    ThemeIconResolver(qreal pixelRatio, InitFlags initFlags = LoadDefaultTheme);

    ThemeIconResolverPrivate *d_ptr;
    Q_DISABLE_COPY(ThemeIconResolver)
    Q_DECLARE_PRIVATE(ThemeIconResolver)

    friend class tst_ThemeIconResolver; // Unit tests
    friend class ThemePrivate;
};
}

#endif // SILICA_THEME_ICON_RESOLVER_H
