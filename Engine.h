#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sys/types.h>
#include <dirent.h>

#include "FileLoop.h"
using namespace stk;

struct RPMFile {
    FileLoop* wav;
    unsigned short rpm;
};

bool sortComp(RPMFile a, RPMFile b);

class Engine
{
    public:
        Engine(const std::string& soundDir);

        virtual ~Engine();

        std::vector<RPMFile>* getPoweredSoundList() { return &poweredSoundList; }
        std::vector<RPMFile>* getReleasedSoundList() { return &releasedSoundList; }

        static std::vector<std::string> listEngines(std::string soundPath);

    private:
        std::string _path;      // Path of the folder containing sound files for an engine. Format filenames like so:
                                // XXXX_P.wav for powered sounds and XXXX.wav for released throttle sounds. XXXX is RPM with leading zeroes if needed
        std::vector<RPMFile> poweredSoundList;       // List of the RPMs file (powered sounds)
        std::vector<RPMFile> releasedSoundList;      // List of the RPMs file (released sounds)
        float rpmRatio;         // Ratio to apply to the car's RPMs to match the recorded engine's. (MAX_RPM_CAR/MAX_RPM_REC_ENGINE)
};

#endif // ENGINE_H
