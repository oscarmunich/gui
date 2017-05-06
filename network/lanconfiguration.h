#ifndef LANCONFIGURATION_H
#define LANCONFIGURATION_H

#include "configuration.h"

class LanConfiguration : public Configuration
{
public:
    LanConfiguration(QObject *parent = 0);
    ~LanConfiguration();
};

#endif // LANCONFIGURATION_H
