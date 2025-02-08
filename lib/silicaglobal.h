/****************************************************************************************
**
** Copyright (C) 2018 Jolla Ltd.
** All rights reserved.
**
** This file is part of Sailfish Silica UI component package.
**
** You may use this file under the terms of the GNU Lesser General
** Public License version 2.1 as published by the Free Software Foundation
** and appearing in the file license.lgpl included in the packaging
** of this file.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file license.lgpl included in the packaging
** of this file.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
****************************************************************************************/

#ifndef SAILFISH_SILICA_GLOBAL_H
#define SAILFISH_SILICA_GLOBAL_H

#include <QtGlobal>

#if defined(SAILFISH_SILICA_BUILD_LIBRARY)
#define SAILFISH_SILICA_EXPORT Q_DECL_EXPORT
#else
#define SAILFISH_SILICA_EXPORT Q_DECL_IMPORT
#endif

#endif // SAILFISH_SILICA_GLOBAL_H
