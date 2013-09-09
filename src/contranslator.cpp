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

#include "contranslator.h"

QString ConTranslator::translate(const char *context, const char *sourceText, const char *disambiguation) const
{
    if (Intercom->IfErrorMsgsDisabled())
        return QTranslator::translate(context, sourceText, disambiguation);
    else
        return "";
}
