#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <sndfile.h>
#include <string>
#include <vector>
#include "Audio.hpp"


void Audio::loadAudio(const std::string &path)
{
	SF_INFO fileInfos;
    SNDFILE* file = sf_open(path.c_str(), SFM_READ, &fileInfos);
    if(!file)
    {   
        std::cerr << "Audiofile has not been openned " << path << std::endl;
        return ;
    }
    // Lecture du nombre d'échantillons et du taux d'échantillonnage (nombre d'échantillons à lire par seconde)
    ALsizei NbSamples  = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
    ALsizei SampleRate = static_cast<ALsizei>(fileInfos.samplerate);
    // Lecture des échantillons audio au format entier 16 bits signé
    std::vector<ALshort> Samples(NbSamples);
    if (sf_read_short(file, &Samples[0], NbSamples) < NbSamples)
    {
        std::cerr << "Audiofile could not be read" << path << std::endl;   
        return ;
    }
    // Fermeture du fichier
    sf_close(file);
    // Détermination du format en fonction du nombre de canaux
    ALenum Format;
	Format = AL_FORMAT_STEREO16;
    // Création du tampon OpenAL
    ALuint Buffer;
    alGenBuffers(1, &Buffer);
    // Remplissage avec les échantillons lus
    alBufferData(Buffer, Format, &Samples[0], NbSamples * sizeof(ALushort), SampleRate);
 
    // Vérification des erreurs
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Audiobuffer has not been created" << path << std::endl;
        return ;
    }
    mPath = path;
    mBuffer = Buffer;
}

void Audio::deleteBuffer()
{
	// Destruction du tampon
    alDeleteBuffers(1, &mBuffer);

    // Destruction de la source
    alSourcei(mSource, AL_BUFFER, 0);
    alDeleteSources(1, &mSource);
}

void Audio::play(int type)
{
	// std::string filename = std::string(path);
	// filename = directory + '/' + filename;
	if (mBuffer == 0)
    {
    	std::cerr << "error: audio:" << mPath << " could not be loaded" << std::endl;
    }
    alGenSources(1, &mSource);
    alSourcei(mSource, AL_BUFFER, mBuffer);
    if(type == 1)
    {
        alSourcei(mSource, AL_LOOPING, 1);
    }
    alSourcePlay(mSource);
    alGetSourcei(mSource, AL_SOURCE_STATE, &mStatus);
    if(mStatus != AL_PLAYING)
    {
        alSourceStop(mSource);
        // //     // // Récupération et affichage de la position courante de lecture en secondes
        // //     // ALfloat Seconds = 0.f;
        // //     // alGetSourcef(Source, AL_SEC_OFFSET, &Seconds);
        // //     // std::cout << "\rLecture en cours... " << std::fixed << std::setprecision(2) << Seconds << " sec";
        // //     // Récupération de l'état du son
        // //    alSourceStop(mSource);    
    } 
}

void Audio::stop()
{
    alSourceStop(mSource);
}

void Audio::pause(){
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
    // Ouverture du device
    ALCdevice* Device = alcOpenDevice(NULL);
    if (!Device)
    {
        std::cerr << "Impossible d'ouvrir le device par défaut" << std::endl;
        return false;
    }

    // Création du contexte
    ALCcontext* Context = alcCreateContext(Device, NULL);
    if (!Context)
    {
        std::cerr << "Impossible de créer un contexte audio" << std::endl;
        return false;
    }

    // Activation du contexte
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
    // Récupération du contexte et du device
    ALCcontext* Context = alcGetCurrentContext();
    ALCdevice*  Device  = alcGetContextsDevice(Context);
 
    // Désactivation du contexte
    alcMakeContextCurrent(NULL);
 
    // Destruction du contexte
    alcDestroyContext(Context);
 
    // Fermeture du device
    alcCloseDevice(Device);
}