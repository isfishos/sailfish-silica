/****************************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** All rights reserved.
**
****************************************************************************************/

/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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

#ifndef SILICAIMAGEPROVIDER_H
#define SILICAIMAGEPROVIDER_H

#include <qquickimageprovider.h>

#include <silicathemeiconresolver.h>

class ThemeTextureAtlas;
class ThemeTextureFactory;

namespace Silica {

class ImageProviderPrivate;

class SAILFISH_SILICA_EXPORT ImageProvider : public QQuickImageProvider
{
public:
    enum InitFlags {
        NoContent = 0x0,
        LoadDefaultTheme = 0x1
    };

    ImageProvider(InitFlags initFlags = NoContent);
    ~ImageProvider() override;

    // Add an icon root to be used for icon lookups.
    //
    // This can include multiple 'zN.M' subdirectories where N.M signify the pixel ratio
    // using two decimals, or one if last is zero. The root and the pixel ratio subdirectories
    // can contain 'icons' and 'icons-monochrome' directories for the icon content.
    void addIconRoot(const QString &path);

    QQuickTextureFactory *requestTexture(const QString &id, QSize *size, const QSize &requestedSize) override;

#ifdef UNIT_TEST
    static QImage colorize(QImage &image, QColor color);
#endif

private:
    ImageProviderPrivate *d_ptr;

    friend class ::ThemeTextureFactory;
};

}

#endif
