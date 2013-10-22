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

#ifndef HOUT_H
#define HOUT_H

//For the sake of reusability this class relies solely on C++ Standard Library
//This class is not aware of multibyte strings! Be careful when using non-SBCS character sets!

#include <string.h>

class Hout {
private:
    static size_t terminal_size;
public:
    virtual ~Hout()=0;  //A trick to make class abstract, non-instantiatable. Works only with destructors.
    static void Paragraph(const char *str, size_t indent=0, const char *padding=NULL);
    static void Separator(const char *str, size_t indent=0, char filler=' ');
    static void EmptyLine();
    static void SetTerminalSize(size_t size);
};

#endif // HOUT_H
