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
