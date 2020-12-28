#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <sndfile.h>
#include <string>
#include <vector>
#include "Audio.hpp"


ALuint Audio::loadAudio(const std::string &path)
{
	SF_INFO fileInfos;
    SNDFILE* file = sf_open(path.c_str(), SFM_READ, &fileInfos);
    if(!file)
    {
        return 0;
    }
    // Lecture du nombre d'échantillons et du taux d'échantillonnage (nombre d'échantillons à lire par seconde)
    ALsizei NbSamples  = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
    ALsizei SampleRate = static_cast<ALsizei>(fileInfos.samplerate);
    // Lecture des échantillons audio au format entier 16 bits signé
    std::vector<ALshort> Samples(NbSamples);
    if (sf_read_short(file, &Samples[0], NbSamples) < NbSamples)
    {
        return 0;
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
        return 0;
    }

    return Buffer;
}

void Audio::deleteBuffer()
{
	// Destruction du tampon
    alDeleteBuffers(1, &mBuffer);

    // Destruction de la source
    alSourcei(mSource, AL_BUFFER, 0);
    alDeleteSources(1, &mSource);
}

ALuint Audio::AudioFromFile(const char *path, const std::string &directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	mBuffer = loadAudio(filename);
	if (mBuffer == 0)
    {
    	std::cerr << "error: audio:" << filename << " could not be loaded" << std::endl;
        return EXIT_FAILURE;
    }
    alGenSources(1, &mSource);
    alSourcei(mSource, AL_BUFFER, mBuffer);
    return mSource;
}

bool InitOpenAL()
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

void ShutdownOpenAL()
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