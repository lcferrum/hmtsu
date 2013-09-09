#ifndef COMMON_H
#define COMMON_H

#define HMTSU_VERSION_STRING "v 0.1.0"
#define HMTSU_COPYRIGHT_STRING "2013 Lcferrum"

#define NORMAL_EXIT_CODE    0
#define ERROR_EXIT_CODE     1
#define CANCELED_EXIT_CODE  2
#define DENIED_EXIT_CODE    3

#define MAX_PSW_ATTEMPTS    3

#ifdef QT_DEBUG
#define IF_DEBUG(x)         x
#define IFNOT_DEBUG(x)
#define CND_DEBUG(x,y)      x
#else
#define IF_DEBUG(x)
#define IFNOT_DEBUG(x)      x
#define CND_DEBUG(x,y)      y
#endif

#endif // COMMON_H
