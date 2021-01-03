#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <sndfile.h>
#include <string>
#include <vector>
#include "Audio.hpp"

void Audio::loadAudio(const std::string &path, std::string type)
{
	SF_INFO fileInfos;
    SNDFILE* file = sf_open(path.c_str(), SFM_READ, &fileInfos);
    if(!file)
    {   
        std::cerr << "Audiofile has not been openned " << path << std::endl;
        return ;
    }
    //Read the number of samples and the samplerate
    ALsizei NbSamples  = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
    ALsizei SampleRate = static_cast<ALsizei>(fileInfos.samplerate);
    //read in 16 bits (most common audio sample radte)
    std::vector<ALshort> Samples(NbSamples);
    if (sf_read_short(file, &Samples[0], NbSamples) < NbSamples)
    {
        std::cerr << "Audiofile could not be read" << path << std::endl;   
        return;
    }
    // Fermeture du fichier
    sf_close(file);
    mPath = path;
    // Détermination du format en fonction du nombre de canaux
    ALenum Format;
	Format = AL_FORMAT_STEREO16;
    // Création du tampon OpenAL
    alGenBuffers(1, &mBuffer);
    // Remplissage avec les échantillons lus
    alBufferData(mBuffer, Format, &Samples[0], NbSamples * sizeof(ALushort), SampleRate);
    // Vérification des erreurs
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Audiobuffer has not been created" << path << std::endl;
        return;
    }
    if (mBuffer == 0)
    {
        std::cerr << "error: audio:" << mPath << " could not be loaded" << std::endl;
    }
    alGenSources(1, &mSource);
    alSourcei(mSource, AL_BUFFER, mBuffer);
    if(type == "LOOPING")
    {
        alSourcei(mSource, AL_LOOPING, 1);
    }
}

void Audio::deleteBuffer()
{
    alDeleteBuffers(1, &mBuffer);
    alSourcei(mSource, AL_BUFFER, 0);
    alDeleteSources(1, &mSource);
}

void Audio::play()
{
    alSourcePlay(mSource);
    alGetSourcei(mSource, AL_SOURCE_STATE, &mStatus);
}

void Audio::stop()
{
    alSourceStop(mSource);
}

void Audio::pause()
{
    alGetSourcei(mSource, AL_SOURCE_STATE, &mStatus);
    if(mStatus == AL_PLAYING)
    {
        alSourcePause(mSource);
    }
    else
    {
        alSourcePlay(mSource);
    }
}

bool initOpenAL()
{
    //Open the device
    ALCdevice* Device = alcOpenDevice(NULL);
    if (!Device)
    {
        std::cerr << "Impossible d'ouvrir le device par défaut" << std::endl;
        return false;
    }
    //Create the context
    ALCcontext* Context = alcCreateContext(Device, NULL);
    if (!Context)
    {
        std::cerr << "Impossible de créer un contexte audio" << std::endl;
        return false;
    }
    //Activate the context
    if (!alcMakeContextCurrent(Context))
    {
        std::cerr << "Impossible d'activer le contexte audio" << std::endl;
        alcDestroyContext(Context);
        alcCloseDevice(Device);
        return false;
    }
    return true;
}

void shutdownOpenAL()
{
    //Get device context
    ALCcontext* Context = alcGetCurrentContext();
    ALCdevice*  Device  = alcGetContextsDevice(Context);
    //Desactivate context
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    //Close the device
    alcCloseDevice(Device);
}