// crtsine.cpp STK tutorial program

#include "SineWave.h"
#include "RtAudio.h"
#include "RtError.h"
#include "FileLoop.h"
using namespace stk;

#include "Engine.h"
#include "DummySensor.h"
#include "Mixer.h"

#define MAX_RPM_CAR     6500        // Max RPMs the car can handle

Mixer mix;

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
    return mix.tick(outputBuffer, inputBuffer, nBufferFrames, streamTime, status, dataPointer);
}

int main()
{
  // Set the global sample rate before creating class instances.
  Stk::setSampleRate( 44100.0 );

  std::vector<std::string> engines = Engine::listEngines("/home/pi/SoundTest/sounds/");
  //if(engines.size() == 0)   goto cleanup;
  RPMSensor dummySensor;
  mix.setSensor(&dummySensor);

    // Select engine
    unsigned int engineIndex = 0;
    while(engineIndex == 0)
    {
        std::cout << "\n\nEngines: ";

        for(unsigned int i = 0 ; i < engines.size() ; i++)
        {
            std::cout << "\n" << i+1 << ". " << engines[i];
        }

        std::cout << "\nEngine selection: ";
        std::string sel;
        std::cin >> sel;
        engineIndex = atoi(sel.c_str());

        if((engineIndex-1) >= engines.size())   engineIndex = 0;
    }

    std::cin.clear();
    std::cin.ignore((unsigned short)-1, '\n');

    Engine selectedEngine(engines[engineIndex-1]);
  mix.loadEngine(&selectedEngine);


  RtAudio dac;

  // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 2;
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  unsigned int bufferFrames = 256;

  try {
    dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)NULL );
  }
  catch ( RtError &error ) {
    error.printMessage();
    goto cleanup;
  }


  try {
    dac.startStream();
  }
  catch ( RtError &error ) {
    error.printMessage();
    goto cleanup;
  }

  // Block waiting here.
  char keyhit;
  std::cout << "\nPlaying ... press enter to quit.\n";
  std::cin.get(keyhit);

  // Shut down the output stream.
  try {
    dac.closeStream();
  }
  catch ( RtError &error ) {
    error.printMessage();
  }

 cleanup:
  return 0;
}
