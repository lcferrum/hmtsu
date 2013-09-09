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

#ifndef RUNMODES_H
#define RUNMODES_H

#include<QObject>

class RunModes {
    Q_GADGET
    Q_ENUMS(QmlEnum)
public:
    enum QmlEnum { SU, SUDO, ARIADNE, PRINT };
};

#endif // RUNMODES_H
