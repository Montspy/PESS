#ifndef MIXER_H
#define MIXER_H

#include "Engine.h"
#include "RPMSensor.h"
#include <cmath>

#include "FileLoop.h"
#include "RtAudio.h"
using namespace stk;


struct MixParams {
    double lowSoundVolume;
    double lowSoundRate;        // Should be 0 if the low sound does not exist (rpm < lowest rpm file).
    RPMFile* lowSound;

    double highSoundVolume;
    double highSoundRate;       // Should be 0 if the high sound does not exist (rpm > highest rpm file).
    RPMFile* highSound;
};

/*

// starting with pan range of -1 to +1,
float angle = (panRange+1)*(0.25*PI); // range 0 to PI/2.
float leftAmplitude = cos(angle);
float rightAmplitude = sin(angle);

*/

class Mixer
{
    public:
        Mixer(Engine* e = NULL);
        virtual ~Mixer();

        void loadEngine(Engine* e);
        //void loadReverse(Reverse r);
        void setSensor(RPMSensor* s);

        int tick(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *dataPointer);
    protected:
    private:
        Engine* engine;
        //Reverse reverse;
        RPMSensor* sensor;

        MixParams computeMix(unsigned int rpm);
        MixParams computeMixCosSin(unsigned int rpm);
        MixParams computeMixLog(unsigned int rpm);

        StkFloat oldSample;
};

#endif // MIXER_H
