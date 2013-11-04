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

#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QDeclarativeImageProvider>

class IconProvider: public QDeclarativeImageProvider {
public:
    IconProvider():
        QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap) {}
    virtual QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // ICONPROVIDER_H
