#include "contranslator.h"

QString ConTranslator::translate(const char *context, const char *sourceText, const char *disambiguation) const
{
    if (Intercom->IfErrorMsgsDisabled())
        return QTranslator::translate(context, sourceText, disambiguation);
    else
        return "";
}
