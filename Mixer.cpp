#include "Mixer.h"

Mixer::Mixer(Engine* e) : engine(e)
{
    oldSample = 0;
}

Mixer::~Mixer()
{

}

void Mixer::loadEngine(Engine* e)
{
    engine = e;
}

void Mixer::setSensor(RPMSensor* s)
{
    sensor = s;
}

int Mixer::tick(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *dataPointer)
{

    //MixParams params = computeMix(sensor->getRPM());
    MixParams params = computeMixCosSin(sensor->getRPM());
    //MixParams params = computeMixLog(sensor->getRPM());

    register StkFloat *samples = (StkFloat *) outputBuffer;
    if(params.highSoundRate > 0)    params.highSound->wav->setRate(params.highSoundRate/2.0);
    if(params.lowSoundRate > 0)     params.lowSound->wav->setRate(params.lowSoundRate/2.0);
    StkFloat sample = 0;

    for ( unsigned int i=0; i<nBufferFrames; i ++) {
        sample = 0;

        if(params.highSoundRate > 0)
        {
            sample += params.highSoundVolume * params.highSound->wav->tick();
        }
        if(params.lowSoundRate > 0)
        {
            sample += params.lowSoundVolume * params.lowSound->wav->tick();
        }

        *(samples++) = sample;
        *(samples++) = sample;

        oldSample = sample;
    }

    return 0;
}

MixParams Mixer::computeMix(unsigned int rpm)
{
    MixParams results;
    bool powered = true;
    std::vector<RPMFile>* sounds;

    if(powered)
    {
        sounds = engine->getPoweredSoundList();
    }
    else
    {
        sounds = engine->getReleasedSoundList();
    }

    if(rpm >= sounds->back().rpm)    // Higher than highest rpm file.
    {
        results.highSound = NULL;
        results.highSoundRate = 0;
        results.highSoundVolume = 0;

        results.lowSound = &sounds->back();
        results.lowSoundRate = double(rpm)/results.lowSound->rpm;
        results.lowSoundVolume = 1;

        //std::cout << " : HIGH";
    }
    else if(rpm <= sounds->front().rpm)  // Lower than lowest rpm file.
    {
        results.highSound = &sounds->front();
        results.highSoundRate = double(rpm)/results.highSound->rpm;
        results.highSoundVolume = 1;

        results.lowSound = NULL;
        results.lowSoundRate = 0;
        results.lowSoundVolume = 0;

        //std::cout << " : LOW ";
    }
    else
    {
        unsigned int index = 0;
        while((*sounds)[index].rpm < rpm)  index++;

        results.highSound = &(*sounds)[index];
        results.lowSound = &(*sounds)[index-1];

        results.highSoundRate = double(rpm)/results.highSound->rpm;
        results.lowSoundRate = double(rpm)/results.lowSound->rpm;

        results.highSoundVolume = double(1 - double(results.highSound->rpm - rpm)/(results.highSound->rpm - results.lowSound->rpm));
        results.lowSoundVolume = double(1 - double(rpm - results.lowSound->rpm)/(results.highSound->rpm - results.lowSound->rpm))*0.9;

        //std::cout << " : STD ";
    }

    //std::cout << " , rates(" << results.highSoundRate << "," << results.lowSoundRate << ")";
    //std::cout << " , volumes(" << results.highSoundVolume << "," << results.lowSoundVolume << ")";

    return results;
}

MixParams Mixer::computeMixCosSin(unsigned int rpm)
{
    MixParams results;
    bool powered = true;
    std::vector<RPMFile>* sounds;

    if(powered)
    {
        sounds = engine->getPoweredSoundList();
    }
    else
    {
        sounds = engine->getReleasedSoundList();
    }

    if(rpm >= sounds->back().rpm)    // Higher than highest rpm file.
    {
        results.highSound = NULL;
        results.highSoundRate = 0;
        results.highSoundVolume = 0;

        results.lowSound = &sounds->back();
        results.lowSoundRate = double(rpm)/results.lowSound->rpm;
        results.lowSoundVolume = 1/sqrt(2);

        //std::cout << " : HIGH";
    }
    else if(rpm <= sounds->front().rpm)  // Lower than lowest rpm file.
    {
        results.highSound = &sounds->front();
        results.highSoundRate = double(rpm)/results.highSound->rpm;
        results.highSoundVolume = 1/sqrt(2);

        results.lowSound = NULL;
        results.lowSoundRate = 0;
        results.lowSoundVolume = 0;

        //std::cout << " : LOW ";
    }
    else
    {
        unsigned int index = 0;
        while((*sounds)[index].rpm < rpm)  index++;

        results.highSound = &(*sounds)[index];
        results.lowSound = &(*sounds)[index-1];

        results.highSoundRate = double(rpm)/results.highSound->rpm;
        results.lowSoundRate = double(rpm)/results.lowSound->rpm;

        StkFloat angle = (0.5*PI) * double(1 - double(results.highSound->rpm - rpm)/(results.highSound->rpm - results.lowSound->rpm));

        results.highSoundVolume = sin(angle)/sqrt(2);
        results.lowSoundVolume = cos(angle)/sqrt(2);

        //std::cout << " : STD ";
    }

    //std::cout << " , rates(" << results.highSoundRate << "," << results.lowSoundRate << ")";
    //std::cout << " , volumes(" << results.highSoundVolume << "," << results.lowSoundVolume << ")";

    return results;
}

MixParams Mixer::computeMixLog(unsigned int rpm)
{
    MixParams results;
    bool powered = true;
    std::vector<RPMFile>* sounds;

    if(powered)
    {
        sounds = engine->getPoweredSoundList();
    }
    else
    {
        sounds = engine->getReleasedSoundList();
    }

    if(rpm >= sounds->back().rpm)    // Higher than highest rpm file.
    {
        results.highSound = NULL;
        results.highSoundRate = 0;
        results.highSoundVolume = 0;

        results.lowSound = &sounds->back();
        results.lowSoundRate = double(rpm)/results.lowSound->rpm;
        results.lowSoundVolume = 1;

        //std::cout << " : HIGH";
    }
    else if(rpm <= sounds->front().rpm)  // Lower than lowest rpm file.
    {
        results.highSound = &sounds->front();
        results.highSoundRate = double(rpm)/results.highSound->rpm;
        results.highSoundVolume = 1;

        results.lowSound = NULL;
        results.lowSoundRate = 0;
        results.lowSoundVolume = 0;

        //std::cout << " : LOW ";
    }
    else
    {
        unsigned int index = 0;
        while((*sounds)[index].rpm < rpm)  index++;

        results.highSound = &(*sounds)[index];
        results.lowSound = &(*sounds)[index-1];

        results.highSoundRate = double(rpm)/results.highSound->rpm;
        results.lowSoundRate = double(rpm)/results.lowSound->rpm;

        StkFloat x = double(1 - double(results.highSound->rpm - rpm)/(results.highSound->rpm - results.lowSound->rpm));

        results.highSoundVolume = log(100*x+1)/log(101)/log(51);
        results.lowSoundVolume  = log(100*(1-x)+1)/log(101)/log(51);

        //std::cout << " : STD ";
    }

    //std::cout << " , rates(" << results.highSoundRate << "," << results.lowSoundRate << ")";
    //std::cout << " , volumes(" << results.highSoundVolume << "," << results.lowSoundVolume << ")";

    return results;
}
