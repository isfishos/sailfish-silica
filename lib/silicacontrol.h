/****************************************************************************************
**
** Copyright (C) 2019 Jolla Ltd.
** Contact: Andrew den Exter <andrew.den.exter@jolla.com>
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

#ifndef SAILFISH_SILICA_CONTROL_H
#define SAILFISH_SILICA_CONTROL_H

#include <silicaitem.h>

namespace Silica {

class ControlPrivate;

class SAILFISH_SILICA_EXPORT Control : public Item
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Control)
public:
    explicit Control(QQuickItem *parent = nullptr);
    explicit Control(ControlPrivate &dd, QQuickItem *parent = nullptr);
    ~Control();
};

}
#endif
