/*
 * HMTsu
 * Copyright (C) 2013 Lcferrum
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QIcon>
#include "iconprovider.h"

QPixmap IconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QPixmap pixmap(QIcon::fromTheme(id).pixmap(requestedSize.width()>0?requestedSize.width():80, requestedSize.height()>0?requestedSize.height():80));

    if (size)
        *size=pixmap.size();

    return pixmap;
}
