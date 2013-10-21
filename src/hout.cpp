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

//For the sake of reusability this class relies solely on C++ Standard Library
//This class is not aware of multibyte strings! Be careful when using non-ASCII characters in strings!

#include "hout.h"
#include <iostream>
#include <iomanip>

using namespace std;

#ifdef _WIN32
size_t Hout::terminal_size=79;	//Default terminal size = 80 columns (79 characters + 1 line break)
#else
size_t Hout::terminal_size=80;	//Default terminal size = 80 characters
#endif

void Hout::Paragraph(const char *str, size_t indent, const char *padding)
{
    if (indent>=terminal_size||terminal_size==0) {
        cout<<(padding?padding:"")<<str<<endl;
        return;
    }

    size_t paragraph_width=terminal_size-indent;
    size_t break_point;
    string output=str;

    for (;;) {
        if (output.length()<=paragraph_width) {
            cout<<setw(indent)<<right<<(padding?string(padding, min(indent, strlen(padding))):"")<<setw(paragraph_width)<<left<<output<<endl;
            break;
        } else {
            if ((break_point=output.rfind(' ', paragraph_width-1))==string::npos) break_point=paragraph_width;
                else break_point++;
            cout<<setw(indent)<<right<<(padding?string(padding, min(indent, strlen(padding))):"")<<setw(paragraph_width)<<left<<output.substr(0, break_point)<<endl;
            output.erase(0, break_point);
        }
        if (padding) padding=NULL;
    }
}

void Hout::Separator(const char *str, size_t indent, char filler)
{
    if (indent>=terminal_size||terminal_size==0) {
        cout<<str<<endl;
        return;
    }

    string padding(indent, ' ');
    string output=str;
    output.resize(terminal_size-indent, filler);
    cout<<padding<<output<<endl;
}

void Hout::EmptyLine()
{
    cout<<endl;
}

void Hout::SetTerminalSize(size_t size)
{
#ifdef _WIN32
    if (size<=0)
        terminal_size=0;
    else
        terminal_size=size-1;
#else
    terminal_size=size;
#endif
}
