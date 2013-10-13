#ifndef RPMSENSOR_H
#define RPMSENSOR_H

#include <iostream>

class RPMSensor
{
    public:
        RPMSensor();
        virtual ~RPMSensor();

        unsigned int getRPM();
    protected:
    private:
};

#endif // RPMSENSOR_H
