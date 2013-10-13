#include "Engine.h"

bool sortComp(RPMFile a, RPMFile b) {
    return (a.rpm < b.rpm);
}

Engine::Engine(const std::string& soundDir)
{
    _path = soundDir;

    // List files in directory
    DIR *dp;
    struct dirent *ep;

    dp = opendir (_path.c_str());
    if (dp != NULL)
    {
        while((ep = readdir(dp))) {
            if(ep->d_type == DT_REG) {
                std::string filename = std::string(ep->d_name);

                if(filename.substr(filename.find_last_of(".") + 1) == "wav") {  // Only .wav files
                    //std::cout << "\n" << filename;

                    unsigned short rpm = strtol(filename.c_str(), NULL, 10);

                    if(rpm != 0) {
                        RPMFile f;
                        f.wav = new FileLoop(_path+filename);
                        f.wav->normalize();
                        f.rpm = rpm;
                        if(filename.find_last_of("_P") != std::string::npos) {
                            poweredSoundList.push_back(f);
                        }
                        else {
                            releasedSoundList.push_back(f);
                        }
                    }
                }
            }
        }

        closedir (dp);
    }
    else
        perror ("Couldn't open the directory");

    std::sort(poweredSoundList.begin() , poweredSoundList.end() , sortComp);
    std::sort(releasedSoundList.begin(), releasedSoundList.end(), sortComp);
}

std::vector<std::string> Engine::listEngines(std::string soundPath)
{
    std::vector<std::string> engines;

    soundPath += std::string("engines/");

    std::cout << "\n" << soundPath;

    // List files in directory
    DIR *dp;
    struct dirent *ep;

    dp = opendir (soundPath.c_str());
    if (dp != NULL)
    {
        while((ep = readdir(dp))) {
            if(ep->d_type == DT_DIR) {
                std::string folderName = std::string(ep->d_name);

                if(folderName != "." && folderName != "..")
                    engines.push_back(soundPath + folderName + "/");
            }
        }

        closedir (dp);
    }
    else
        perror ("Couldn't open the directory");

    return engines;
}

Engine::~Engine()
{
    for(unsigned int i = 0 ; i < poweredSoundList.size() ; i++) {
        poweredSoundList[i].wav->closeFile();
        delete poweredSoundList[i].wav;
    }

    for(unsigned int i = 0 ; i < releasedSoundList.size() ; i++) {
        releasedSoundList[i].wav->closeFile();
        delete releasedSoundList[i].wav;
    }
}
