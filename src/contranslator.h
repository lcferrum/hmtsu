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

#ifndef CONTRANSLATOR_H
#define CONTRANSLATOR_H

#include <QTranslator>
#include "commhandler.h"

class ConTranslator: public QTranslator, protected IntercomHandler {
public:
    ConTranslator():
        QTranslator() {}
    virtual QString translate(const char *context, const char *sourceText, const char *disambiguation=0) const;
};

#endif // CONTRANSLATOR_H
