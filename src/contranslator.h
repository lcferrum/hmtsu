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
