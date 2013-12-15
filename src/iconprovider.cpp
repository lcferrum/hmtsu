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
#include <MGConfItem>
#include <unistd.h>
#include "iconprovider.h"

QPixmap IconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QPixmap pixmap(QIcon::fromTheme(id).pixmap(requestedSize.width()>0?requestedSize.width():80, requestedSize.height()>0?requestedSize.height():80));

    if (size)
        *size=pixmap.size();

    return pixmap;
}

void IconProvider::SetToCurrentSystemTheme()
{
    MGConfItem CurrentTheme("/meegotouch/theme/name", NULL);    //By default QIcon theme is hicolor (default fallback theme) - set it to current theme from GConf
    QIcon::setThemeName(CurrentTheme.value().toString());
}

QString IconProvider::ConvertPath(const QString &icon_path)
{
    if (icon_path.length()>0)
        return (access(icon_path.toLocal8Bit().constData(), R_OK)?
                    (QIcon::hasThemeIcon(icon_path)?"image://icon/":"image://theme/"):
                    "file://")
               +icon_path;
    else
        return "";
}
