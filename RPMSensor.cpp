#include "RPMSensor.h"

RPMSensor::RPMSensor()
{

}

RPMSensor::~RPMSensor()
{

}

unsigned int RPMSensor::getRPM()
{
    static unsigned int rpm = 700;

    rpm += 1;

    //std::cout << "\nRpm: " << rpm;
    return rpm;
}
